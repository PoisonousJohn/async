#Async

##Purpose

Async is a module for asynchronous operations designed to use in games. My purpose was to create a custom thread pool that will allow to enqueue tasks to SEPARATE threads. E.g. you can enqueue resource loading tasks to thread 1, network tasks to thread 2 and etc.

This is extremely important in mobile apps due to limited processor resources. We can't create lot of threads and priority of the tasks is not enough, because there can be situtation when all workers are busy with resource loading tasks and you want to do a network query as soon as possible, but worker won't do highest priority task while it is doing other job and you have to wait.

Separating tasks by concrete threads solves this problem. Network tasks that run in thread 1 won't block other tasks running in thread 2

##Simple example

#include <iostream>
#include <vector>
#include "Async.h"


    int main(int argc, const char * argv[])
    {
        Async::Async async;
    
        async.doSync([]() {
            std::cout << "Synchronous task" << std::endl;
        });
    
        async.doAsync([](){
            std::cout << "Asynchronous task" << std::endl;
        }, [](){
            std::cout << "Synchronous callback" << std::endl;
        });
    
        bool stop = false;
    
        std::thread addLater([&](){
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));
            async.doAsync([](){
                std::cout << "later" << std::endl;
            }, [&](){
                std::cout << "later fg" << std::endl;
                stop = true;
            });
        });
    
        addLater.detach();
    
        std::thread asyncThread([&]{
            while (!stop) {
                // your engine should invoke this method every frame
                // this method finishes tasks and calls synchronous callbacks
                async.update();
            }
        });
    
        asyncThread.join();
    
        return 0;
    }

