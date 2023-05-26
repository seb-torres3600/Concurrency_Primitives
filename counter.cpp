#include "counter.hpp"

//constructor
Counter :: Counter(string c_type, int THREADS,int ITERATIONS, string type){
    counter_type = type;
    NUM_ITERATIONS = ITERATIONS;
    NUM_THREADS = THREADS;
    lock_bar_type = c_type;
    bar = new barrier(NUM_THREADS);
}

//delete barrier
Counter :: ~Counter(){
    delete bar;
}


// lock implementation of counter
void Counter :: counter_by_lock(int tid, Locks* lk){


    if(tid == 1){
        clock_gettime(CLOCK_MONOTONIC, &startTime);
    }

    for(int i = 0; i < NUM_ITERATIONS; i++){
        lk->acquire();
        counter++;
        lk->release();
    }
    // need this so that we don't exit the func and delete threads before
    // they're done
    bar -> arrive_and_wait();

    if(tid==1){
        clock_gettime(CLOCK_MONOTONIC,&endTime);
    }

}

// counter with barriers
void Counter :: counter_by_barrier(int tid, Bar* bar){

    bar->arrive_and_wait();

    if(tid == 1){
        clock_gettime(CLOCK_MONOTONIC, &startTime);
    }

    tid--;
    for(int i = 0; i < NUM_ITERATIONS*NUM_THREADS; i++){
        if(i % NUM_THREADS == tid){
            counter++;
        }
        bar->arrive_and_wait();
    }

    if(tid==1){
        clock_gettime(CLOCK_MONOTONIC,&endTime);
    }
}

// main driver function
int Counter :: counter_driver(){
    int i;
    // if lock call counter_lock
    if(counter_type == "lock"){
        Locks my_lock(lock_bar_type);
        threads.resize(NUM_THREADS);
        for(i = 1; i < NUM_THREADS; i++){
            threads[i] = new thread(&Counter:: counter_by_lock,this, i+1, &my_lock);
        }
        i = 1;
        counter_by_lock(i, &my_lock);
    }
    //if bar call counter_bar
    if(counter_type == "bar"){
        Bar bar(NUM_THREADS, lock_bar_type);
        threads.resize(NUM_THREADS);
        for(i = 1; i < NUM_THREADS; i++){
            threads[i] = new thread(&Counter:: counter_by_barrier,this, i+1, &bar);
        }
        i = 1;
        counter_by_barrier(i, &bar);

    }
    // join threads
    for(i = 1; i < NUM_THREADS; i++) {
        threads[i]->join();
        delete threads[i];
    }

    //copied from example lab 1
    unsigned long long elapsed_ns;
    elapsed_ns = (endTime.tv_sec-startTime.tv_sec)*1000000000 + (endTime.tv_nsec-startTime.tv_nsec);
    printf("Elapsed (ns): %llu\n",elapsed_ns);
    double elapsed_s = ((double)elapsed_ns)/1000000000.0;
    printf("Elapsed (s): %lf\n",elapsed_s);

    // retunr count
    return counter;
}