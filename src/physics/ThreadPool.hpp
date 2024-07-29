#pragma once

#include <functional>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

class TaskQueue
{
public:
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_mutex;
    std::atomic<int> m_remaining_tasks = 0;

    template <typename TCallback>
    void addTask(TCallback &&callback)
    {
        std::lock_guard<std::mutex> lock_guard{m_mutex};
        m_tasks.push(std::forward<TCallback>(callback));
        m_remaining_tasks++;
    }

    void getTask(std::function<void()> &target_callback)
    {
        {
            std::lock_guard<std::mutex> lock_guard{m_mutex};
            if (m_tasks.empty())
            {
                return;
            }
            target_callback = std::move(m_tasks.front());
            m_tasks.pop();
        }
    }

    static void wait()
    {
        std::this_thread::yield();
    }

    void waitForCompletion() const
    {
        while (m_remaining_tasks > 0)
        {
            wait();
        }
    }

    void workDone()
    {
        m_remaining_tasks--;
    }
};

class Worker
{
public:
    int m_id = 0;
    std::thread m_thread;
    std::function<void()> m_task = nullptr;
    bool m_running = true;
    TaskQueue *m_queue = nullptr;

    Worker() = default;

    Worker(TaskQueue &queue, int id)
        : m_id{id}, m_queue{&queue}
    {
        m_thread = std::thread([this]()
                               { run(); });
    }

    void run()
    {
        while (m_running)
        {
            m_queue->getTask(m_task);
            if (m_task == nullptr)
            {
                TaskQueue::wait();
            }
            else
            {
                m_task();
                m_queue->workDone();
                m_task = nullptr;
            }
        }
    }

    void stop()
    {
        m_running = false;
        m_thread.join();
    }
};

class ThreadPool
{
public:
    int m_thread_count = 0;
    TaskQueue m_queue;
    std::vector<Worker> m_workers;

    ThreadPool(int thread_count)
        : m_thread_count{thread_count}
    {
        m_workers.reserve(thread_count);
        for (int i{thread_count}; i--;)
        {
            m_workers.emplace_back(m_queue, static_cast<int>(m_workers.size()));
        }
    }

    virtual ~ThreadPool()
    {
        for (Worker &worker : m_workers)
        {
            worker.stop();
        }
    }

    template <typename TCallback>
    void addTask(TCallback &&callback)
    {
        m_queue.addTask(std::forward<TCallback>(callback));
    }

    void waitForCompletion() const
    {
        m_queue.waitForCompletion();
    }
};