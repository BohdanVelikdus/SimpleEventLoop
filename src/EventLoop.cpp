#include "EventLoop.h"

#include <chrono>
#include <thread>
#include <iostream>

using namespace std::literals;

EventLoop::EventLoop()
{
    this->m_ListenThread.reset( new std::thread([this](){
        this->Listening();
    }));
}

void EventLoop::addTask(std::function<void()> f)
{
    std::unique_lock<std::mutex> lock;
    this->m_TaskQueue.push(f);
    lock.release();
}

void EventLoop::Listening()
{
    while(this->m_Active)
    {
        std::unique_lock<std::mutex> lock(this->m_mtx);
        this->m_SignalVariable.wait(lock, [this](){return !this->m_TaskQueue.empty() || this->m_Active; });
        std::this_thread::sleep_for(5s);
        if(!this->m_IsThreadFree)
        {
            while (!this->m_IsThreadFree.load(std::memory_order_acquire) && !this->m_IsThreadFree ) {
               std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
            }
        }

        std::function<void()> Task = m_TaskQueue.front();
        if(Task == nullptr)
            continue;
        m_TaskQueue.pop();
        if(this->m_Active)
        {
            if(this->m_ExecutionThread.get() != nullptr)
            {
                if(this->m_ExecutionThread->joinable())
                    this->m_ExecutionThread->join();
            }    
                
            this->m_ExecutionThread.reset(new std::thread([this, Task = std::move(Task)](){
                std::cout << "Entered thread\n=======\n";
                this->m_IsThreadFree = false;
                Task();
                this->m_IsThreadFree = true;
            }));
        }
    }   
}

void EventLoop::Notify()
{
    this->m_SignalVariable.notify_one();
}

void EventLoop::changeStatus()
{
    this->m_Active = false;
}

EventLoop::~EventLoop()
{
    if((this->m_ListenThread)->joinable())
        (this->m_ListenThread)->join();
    if((this->m_ExecutionThread)->joinable())
        (this->m_ExecutionThread)->join();
    
}
