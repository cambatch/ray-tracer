#include "threadPool.hpp"
#include <thread>

#include <iostream>


ThreadPool::ThreadPool(size_t numThreads) {
    m_Stop = false;

    for (size_t i = 0; i < numThreads; ++i) {
        m_Workers.emplace_back([this] { this->Work(); });
    }
}

ThreadPool::~ThreadPool() {
    Stop();
}

void ThreadPool::EnqueueTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(m_QueueMutex);
        m_Tasks.push(std::move(task));
    }

    m_Condition.notify_one();
}

void ThreadPool::Stop() {
    {
        std::unique_lock<std::mutex> lock(m_QueueMutex);
        if (m_Stop) return;
        m_Stop = true;
    }

    m_Condition.notify_all();

    for (auto& worker : m_Workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::Work() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            m_Condition.wait(lock, [this] { return m_Stop || !m_Tasks.empty(); });

            if (m_Stop) return;

            if (!m_Tasks.empty()) {
                task = std::move(m_Tasks.front());
                m_Tasks.pop();
            }
        }

        if (task) {
            task();
        }
    }

    std::cout << "Thread " << std::this_thread::get_id() << "Exiting Work()\n";
}
