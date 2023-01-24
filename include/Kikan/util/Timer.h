#ifndef KIKAN_TIMER_H
#define KIKAN_TIMER_H

#include <string>
#include <iostream>
#include <chrono>

namespace Kikan{
    class Timer{
    public:
        Timer(std::string tag) : _tag(tag){
            _time = std::chrono::high_resolution_clock::now();
        };
        ~Timer(){
            std::cout << "[TIMER] "
            << ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - _time)).count() << " - "
            << _tag << std::endl;
        };

    private:
        std::string _tag;
        std::chrono::high_resolution_clock::time_point _time;
    };
}

#endif //KIKAN_TIMER_H
