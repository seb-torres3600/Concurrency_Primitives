#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "locks.hpp"
#include "bar.hpp"

using namespace std;

class Counter{
    private:
        struct timespec startTime, endTime;
        int NUM_THREADS;
        int NUM_ITERATIONS;
        string counter_type;
        string lock_bar_type;
        int counter = 0;
        vector<thread*> threads;
        barrier<> *bar;

    public:
        Counter(string c_type, int THREADS,int ITERATIONS,string type);
        ~Counter();
        void counter_by_lock(int tid, Locks* lk);
        void counter_by_barrier(int tid, Bar* bar);
        int counter_driver();
};