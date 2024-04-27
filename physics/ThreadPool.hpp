#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool
{
public:
    ThreadPool(int count);
    std::future<void> enqueue(std::function<void()> f);
    ~ThreadPool();

private:
    std::vector<std::thread> workers;

    std::queue<std::function<void()>> tasks;

    // sync
    std::mutex q_mutex;
    std::condition_variable condition;
    bool stop;
};