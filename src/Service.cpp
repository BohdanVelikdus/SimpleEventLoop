#include "Service.h"

#include <chrono>
#include <thread>

using namespace std::literals;

void Service::addProcessing()
{
    auto self(shared_from_this());
    std::function<void()> func = [this, self]()
    {   
        std::cout << "Continue processing obj num: " << this->m_num <<  "_...\n";
        std::this_thread::sleep_for(1s);
        this->addProcessing(); 
    }; 
    if(this->m_status)
    {
        this->m_EventLoop.addTask(func);
        this->m_EventLoop.Notify();
    }   
}

void Service::changeStatus()
{
    this->m_status = false;
}
Service::~Service()
{
    std::cout << "Destructor is called in Service._.\n";
}
