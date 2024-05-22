#ifndef PYLIKE_DATETIME_H
#define PYLIKE_DATETIME_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "./str.h"





namespace datetime {

    class Datetime {
    public:
        // 构造函数，默认当前时间
        Datetime() {
            now();
        }

        Datetime now() {
            tp_ = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(tp_);
            tm_ = *std::localtime(&now_c);
            return *this;
        }

        pystring strftime(pystring format="") {
            if (!format.length() || format.empty()) {
                format = "%Y-%m-%d %H:%M:%S.%ms";
            }
            std::ostringstream oss, oss_ms;
            auto duration = tp_.time_since_epoch();
            auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            oss_ms << std::setfill('0') << std::setw(3) << millis % 1000;
            format = format.replace("%ms", oss_ms.str());
            oss << std::put_time(&tm_, format.c_str());
            return pystring(oss.str());
        }

        int year() const {
            return tm_.tm_year + 1900;
        }

        int month() const {
            return tm_.tm_mon + 1;
        }

        int day() const {
            return tm_.tm_mday;
        }

        int hour() const {
            return tm_.tm_hour;
        }

        int minute() const {
            return tm_.tm_min;
        }

        int second() const {
            return tm_.tm_sec;
        }

    private:
        std::tm tm_;
        std::chrono::system_clock::time_point tp_;
    };


    namespace datetime {

        Datetime now() {
            return Datetime();
        }
    }

}





#endif