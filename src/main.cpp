#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include <memory>

#include "Service.h"
#include "EventLoop.h"

using namespace std::literals;

std::vector<std::weak_ptr<Service>> services;
void notifyServices()
{
    std::cout << "Ending the tasks...\n";
    for(auto& service : services)
    {
        if(!service.expired())
        {
            std::cout << "Shared_ptr still exists, calling a stop...\n";
            auto shared_service = service.lock();
            shared_service->changeStatus();
        }
        else
        {
            std::cout << "Object expired._.\n";
        }
    }
}

int main()
{
    EventLoop eventLoop;
    std::cout << "If enter \"end\" to end all the task and loops\n";    
    {
        auto firstSharedService = std::make_shared<Service>(1, eventLoop);
        firstSharedService->addProcessing();
        auto secondSharedService = std::make_shared<Service>(2, eventLoop);
        secondSharedService->addProcessing();
        services.push_back(firstSharedService);
        services.push_back(secondSharedService);
    }
    std::cout << "Survived the curly brackets\n";
    std::string info;
    std::cin >> info;
    bool running = true;
    while(running)
    {
        if(info == "end")
        {
            std::cout << "Ending\n...";
            running = false;
            notifyServices();
            break;
        }
        else
        {
            std::cout << "Go further....";
        }
        std::cin >> info;
    }
    eventLoop.changeStatus();   
    return 0;
}