#include "locks.hpp"

// Constructor
Locks ::Locks(std::string lk_type) {
    lock_type = lk_type;
    if(lock_type == "tas"){
        flag.store(false, RELAXED);
    }
    if(lock_type == "ttas"){
        flag.store(false, RELAXED);
    }
    if(lock_type == "ticket"){
        next_num.store(0,RELAXED);
        now_serving.store(0, RELAXED);
    }
    if(lock_type == "pthread") {
        lk = new mutex();
    }
}

// delete lock if pthread
Locks :: ~Locks() {
    if(lock_type == "pthread"){
        delete lk;
    }
}

// Atomically check if value is false, if it is change to true
bool Locks :: TestAndSet() {
    bool expected = false;
    // syntax(expected value, value to swap it with, memory order sync)
    return flag.compare_exchange_strong(expected, true, SEQ_CST);
}
// test and set
void Locks :: tas_lock() {
    while(TestAndSet() == false){}
}

void Locks :: tas_unlock() {
    flag.store(false, SEQ_CST);
}

//test test and set lock
void Locks :: ttas_lock() {
    while(flag.load(SEQ_CST) == true || TestAndSet() == false);
}

//test test and set unlock
void Locks ::ttas_unlock() {
    flag.store(false, SEQ_CST);
}

// ticket lock
void Locks :: ticket_lock() {
    int my_num = next_num.fetch_add(1, SEQ_CST);
    while(now_serving.load(SEQ_CST) != my_num){}
}

// ticket unlock
void Locks :: ticket_unlock(){
    now_serving.fetch_add(1,SEQ_CST);
}

// driver that calls appropriate lock
void Locks ::acquire() {
    if(lock_type == "tas") {tas_lock();}
    if(lock_type == "ttas"){ttas_lock();}
    if(lock_type == "ticket"){ticket_lock();}
    if(lock_type == "pthread"){lk->lock();}
}

// driver that calls appropriate unlock
void Locks ::release() {
    if(lock_type == "tas"){tas_unlock();}
    if(lock_type == "ttas"){ttas_unlock();}
    if(lock_type == "ticket"){ticket_unlock();}
    if(lock_type == "pthread"){lk->unlock();}
}