#include <atomic>
#include <iostream>
#include <string>
#include <mutex>
#include <barrier>

using namespace std;

#define SEQ_CST memory_order_seq_cst
#define RELAXED memory_order_relaxed

class Bar{
    private:
        atomic<int> cnt;
        atomic<int> sense;
        barrier<> *bar;
        void sense_wait();
        int NUM_THREADS;
        string bar_type;

    public:
        Bar(int num_threads, string barrier_type);
        ~Bar();
        void arrive_and_wait();
};