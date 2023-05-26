#include <iostream>
#include <atomic>
#include <string>
#include <mutex>

using namespace std;

#define SEQ_CST memory_order_seq_cst
#define RELAXED memory_order_relaxed

class Locks{
    private:
        string lock_type;
        mutex* lk;
        atomic<bool> flag = false;
        atomic<int> next_num;
        atomic<int> now_serving;
        bool TestAndSet();
        void tas_lock();
        void tas_unlock();
        void ttas_lock();
        void ttas_unlock();
        void ticket_lock();
        void ticket_unlock();

    public:
        Locks(string lk_type);
        ~Locks();
        void acquire();
        void release();
};
