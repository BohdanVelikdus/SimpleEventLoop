#pragma once

#include <iostream>
#include <memory>
#include <functional>
#include <atomic>


#include "EventLoop.h"

class Service : public std::enable_shared_from_this<Service>
{
public:
    Service(int num, EventLoop& el) : m_num(num), m_EventLoop(el) {}

    void addProcessing();
    void changeStatus();

    ~Service();

private:

    EventLoop& m_EventLoop;

    int m_num;

    std::atomic_bool m_status{true};
};