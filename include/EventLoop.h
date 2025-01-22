#pragma once

#include <memory>
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <atomic>

class EventLoop
{
public:
    EventLoop(); 

    void addTask(std::function<void()> f);

    void Notify();

    void changeStatus();

    ~EventLoop();

private:

    void Listening();

private:

    std::unique_ptr<std::thread> m_ExecutionThread;
    std::atomic_bool m_IsThreadFree{true};

    std::unique_ptr<std::thread> m_ListenThread;
    
    std::queue<std::function<void()>> m_TaskQueue;

    std::condition_variable m_SignalVariable;
    std::mutex m_mtx;
    std::atomic_bool m_Active{true};
};