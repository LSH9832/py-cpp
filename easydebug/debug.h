#ifndef EASYDEBUG_H
#define EASYDEBUG_H

#define BLUE "\033[34m"   /* Blue   */
#define YELLOW "\033[33m" /* Yellow */
#define GREEN "\033[32m"  /* Green  */
#define RED "\033[31m"    /* Red    */
#define END "\033[0m"
#define ENDL "\033[0m" << std::endl

#define DEBUG (std::cout << BLUE << "[DEBUG] ")
#define WARN (std::cout << YELLOW << "[WARN] ")
#define INFO (std::cout << GREEN << "[INFO] ")
#define ERROR (std::cout << RED << "[ERROR] ") 

#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#define MAX(a,b)  ((a) < (b) ? (b) : (a))

#include <sys/time.h>
#include <cstddef>
#include <vector>
#include <iostream>


namespace debug {

    static timeval get_now_time() {
        timeval _t;
        gettimeofday(&_t, NULL);
        return _t;
    }

    static int get_time_interval(timeval _t1, timeval _t2) {
        return (int)((_t2.tv_sec - _t1.tv_sec) * 1000 + (_t2.tv_usec - _t1.tv_usec) / 1000);
    }

    static float get_time_interval_f(timeval _t1, timeval _t2) {
        return ((float)(_t2.tv_sec - _t1.tv_sec) * 1000. + (float)(_t2.tv_usec - _t1.tv_usec) / 1000.);
    }

    static void sleep(float minisec) {
        timeval t0 = get_now_time();
        while (1) {
            if (get_time_interval_f(t0, get_now_time()) > minisec) break;
        }
    }

    class TimeCount {
        std::vector<timeval> tics;
        std::vector<timeval> tocs;

    public:

        size_t length() {
            return tics.size();
        }
        
        void tic(int idx) {
            timeval now_time = get_now_time();
            while (idx >= this->length()) {
                tics.push_back(now_time);
                tocs.push_back(now_time);
            }
            tics[idx] = now_time;
            tocs[idx] = now_time;
        }

        int get_timeval(int idx) {
            idx = MIN(idx, this->length()-1);
            return get_time_interval(tics[idx], tocs[idx]);
        }

        float get_timeval_f(int idx) {
            idx = MIN(idx, this->length()-1);
            return get_time_interval_f(tics[idx], tocs[idx]);
        }

        int toc(int idx) {
            idx = MIN(idx, this->length()-1);
            tocs[idx] = get_now_time();
            return this->get_timeval(idx);
        }
    
    };

}

#endif
