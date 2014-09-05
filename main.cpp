#include <iostream>
#include <vector>
#include "Async.h"

using namespace poison;

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