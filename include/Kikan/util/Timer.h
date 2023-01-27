#ifndef KIKAN_TIMER_H
#define KIKAN_TIMER_H

#include <string>
#include <iostream>
#include <chrono>
#include <utility>

namespace Kikan{
    class Timer{
    public:
        enum Precision{
            MICRO,
            MILLI,
            MINUTES
        };

        explicit Timer(std::string tag, Precision p = Precision::MILLI) : _tag(std::move(tag)), _p(p) {
            start();
        };
        explicit Timer(long long* t, Precision p = Precision::MILLI) : _t(t), _p(p) {
            start();
        };

        ~Timer(){
            stop();
            long long duration = time(_p);
            if(_t){
                *_t = duration;
            }
            else{
                std::cout << "[TIMER] " << duration << " - " << _tag << std::endl;
            }
        };

        void start(){
            _start = std::chrono::high_resolution_clock::now();
        };
        void stop(){
            _stop = std::chrono::high_resolution_clock::now();
        };
        long long time(Precision p = Precision::MILLI){
            switch (p) {
                case MILLI:{
                    auto start =  std::chrono::time_point_cast<std::chrono::milliseconds>(_start).time_since_epoch().count();
                    auto stop =  std::chrono::time_point_cast<std::chrono::milliseconds>(_stop).time_since_epoch().count();
                    return stop - start;
                }
                case MICRO:{
                    auto start =  std::chrono::time_point_cast<std::chrono::microseconds>(_start).time_since_epoch().count();
                    auto stop =  std::chrono::time_point_cast<std::chrono::microseconds>(_stop).time_since_epoch().count();
                    return stop - start;
                }
                case MINUTES:{
                    auto start =  std::chrono::time_point_cast<std::chrono::minutes>(_start).time_since_epoch().count();
                    auto stop =  std::chrono::time_point_cast<std::chrono::minutes>(_stop).time_since_epoch().count();
                    return stop - start;
                }
            }

            return 0;
        }

    private:
        std::string _tag;
        long long* _t = nullptr;

        Precision _p;

        std::chrono::time_point<std::chrono::high_resolution_clock> _start;
        std::chrono::time_point<std::chrono::high_resolution_clock> _stop;
    };
}

#endif //KIKAN_TIMER_H
