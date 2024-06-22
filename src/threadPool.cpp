#include "threadPool.hpp"


ThreadPool::ThreadPool(size_t numThreads) {
    m_Stop = false;

    for (size_t i = 0; i < numThreads; ++i) {
        m_Workers.emplace_back([this] { this->Work(); });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(m_QueueMutex);
        m_Stop = true;
    }

    m_Condition.notify_all();

    for (auto& worker : m_Workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::EnqueueTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(m_QueueMutex);
        m_Tasks.push(std::move(task));
    }

    m_Condition.notify_one();
}

void ThreadPool::WaitForCompletion() {
    std::unique_lock<std::mutex> lock(m_QueueMutex);
    m_Condition.wait(lock, [this] { return m_Tasks.empty(); });
}

void ThreadPool::Work() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            m_Condition.wait(lock, [this] { return m_Stop || !m_Tasks.empty(); });

            if (m_Stop && m_Tasks.empty()) {
                return;
            }

            task = std::move(m_Tasks.front());
            m_Tasks.pop();
        }

        task();
    }
}
