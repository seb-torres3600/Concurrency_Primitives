#include "bar.hpp"

// Constructor, takes in number of threads, and barrier type wanted
Bar :: Bar(int num_threads, string barrier_type){
    NUM_THREADS = num_threads;
    bar_type = barrier_type;

    if(bar_type == "sense"){
        cnt.store(0, RELAXED);
        sense.store(0, RELAXED);
    }
    // if pthread call barrier function from C++
    if(bar_type == "pthread"){
        bar = new barrier(NUM_THREADS);
    }
}

Bar :: ~Bar(){
    // delete barrier if we are using pthread
    if(bar_type == "pthread"){
        delete bar;
    }
}


void Bar :: sense_wait(){
    // each thread has a my_sense var
    thread_local bool my_sense = 0;
    if(my_sense == 0){
        my_sense = 1;
    }
    else{
        my_sense = 0;
    }

    int cnt_cpy = cnt.fetch_add(1, SEQ_CST);
    // once all threads have flipped there my sense variable
    // then release the threads
    if(cnt_cpy == NUM_THREADS - 1){
        cnt.store(0, RELAXED);
        sense.store(my_sense, SEQ_CST);
    }
    //otherwise keep in while loop waiting until all threads arrive
    else{
        while(sense.load(SEQ_CST) != my_sense){}
    }

}

// driver function
// if sense barrier than call my implementation
//otherwise call c++ arrive_and_wait
void Bar :: arrive_and_wait(){
    if(bar_type == "sense"){
        sense_wait();
    }
    else if(bar_type == "pthread"){
        bar->arrive_and_wait();
    }
    else{
        cout << "Barrier error encountered" << endl;
        cout << bar_type << endl;
        return;
    }
}