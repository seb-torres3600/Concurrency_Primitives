#include "sort.hpp"

// init barrier, mutex, and number of threads
BucketSort ::BucketSort(int num_of_threads, string l_type, string b_type) {
    lock_type = l_type;
    bar_type = b_type;
    NUM_THREADS = num_of_threads;
}


// each threads calls this
void BucketSort ::Bucket(int tid, Locks* lk, Bar* bar) {
    // wait for all to arrive
    bar->arrive_and_wait();
    // main thread get start time
    if(tid == 1){
        clock_gettime(CLOCK_MONOTONIC, &startTime);
    }
    // wait for work to complete
    bar->arrive_and_wait();

    // Handle the last chunk of data
    if(tid == NUM_THREADS) {
        int starting_index = (tid-1) * split_size;
        for(int i = starting_index; i < int(complete_data.size()); i++) {
            // get bucket we need to add data into
            // Ex: floor((4 threads - 1 = number of buckets)* 12(data we are at)/MAX = 29) = bucket 1
            // Bucket data in that case is split [0-7.25][7.26-14.5][14.51-21.75][21.75-29]
            int bucket_index = floor((NUM_THREADS-1) * complete_data[i] / MAX);
            // Lock bucket so no other threads can access it
            lk->acquire();
            // insert data into appropriate bucket
            buckets[bucket_index].insert({complete_data[i], 0});
            lk->release();
        }
    }

        // Sorting chunks of the data, keeping track using the thread id
    else {
        int starting_index = (tid-1) * split_size;
        int ending_index = ((tid-1) * split_size)+ split_size;
        for(int i = starting_index; i < ending_index; i++) {
            int bucket_index = floor((NUM_THREADS-1) * complete_data[i] / MAX);
            // Lock bucket so no other threads can access it
            lk->acquire();
            // insert data into appriopriate bucket which is a map
            buckets[bucket_index].insert({complete_data[i], 0});
            lk->release();
        }
    }

    bar->arrive_and_wait();

    if(tid==1){
        clock_gettime(CLOCK_MONOTONIC,&endTime);
    }

    return;
}

void BucketSort :: MainDriver(vector<int> &data) {

    Locks my_lock(lock_type);
    Bar my_bar(NUM_THREADS, bar_type);

    // get appropriate number of maps in our vector
    for(int i = 0; i < NUM_THREADS;i++){
        buckets.push_back(map<int,int>());
    }

    // get split size
    split_size = round(data.size()/NUM_THREADS);

    complete_data = data;
    // https://www.includehelp.com/stl/find-the-maximum-largest-element-of-a-vector.aspx
    MAX = *max_element(data.begin(), data.end());

    // resize thread vector to number of threads
    threads.resize(NUM_THREADS);

    int i;
    // create new threads and start at Bucket function
    for(i = 1;i < NUM_THREADS;i++){
        threads[i] = new thread(&BucketSort::Bucket,this, i+1, &my_lock, &my_bar);
    }

    i = 1;
    // main thread should also call Bucket
    Bucket(i,&my_lock,&my_bar);

    // join threads and delete thread
    for(int i =1; i < NUM_THREADS;i++){
        threads[i]->join();
        delete threads[i];
    }

    // merge the map keys into the final vector
    int index = 0;
    // https://www.techiedelight.com/print-keys-values-map-cpp/
    for(int i = 0; i < NUM_THREADS; i++) {
        for (auto const &pair: buckets[i]) {
            data[index] = pair.first;
            index++;
        }
    }

    //copied from example
    unsigned long long elapsed_ns;
    elapsed_ns = (endTime.tv_sec-startTime.tv_sec)*1000000000 + (endTime.tv_nsec-startTime.tv_nsec);
    printf("Elapsed (ns): %llu\n",elapsed_ns);
    double elapsed_s = ((double)elapsed_ns)/1000000000.0;
    printf("Elapsed (s): %lf\n",elapsed_s);
}


