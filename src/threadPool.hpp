#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>


class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    void EnqueueTask(std::function<void()> task);
    void WaitForCompletion();

    void Stop();
    bool Working() const { return !m_Stop; }

private:
    void Work();

private:
    std::vector<std::thread> m_Workers;
    std::mutex m_QueueMutex;
    std::queue<std::function<void()>> m_Tasks;
    std::condition_variable m_Condition;
    bool m_Stop;
};
