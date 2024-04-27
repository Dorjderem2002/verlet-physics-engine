#include "ThreadPool.hpp"

ThreadPool::ThreadPool(int count) : stop(false)
{
    for (int i = 0; i < count; i++)
    {
        workers.emplace_back([this]
                             { 
            for(;;)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->q_mutex);
                    this->condition.wait(lock, [this] {return this->stop || !this->tasks.empty();});
                    if(this->stop && this->tasks.empty()) return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            } });
    }
}

std::future<void> ThreadPool::enqueue(std::function<void()> f)
{
    std::shared_ptr<std::packaged_task<void()>> task = std::make_shared<std::packaged_task<void()>>(std::forward<std::function<void()>>(f));
    std::future<void> result = task->get_future();
    {
        std::unique_lock<std::mutex> lock(q_mutex);
        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        tasks.emplace([task]()
                      { (*task)(); });
    }
    condition.notify_one();
    return result;
}

inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(q_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers)
        worker.join();
}