#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <map>
#include <cmath>
#include "locks.hpp"
#include "bar.hpp"

using namespace std;

class BucketSort{
    private:
        struct timespec startTime, endTime;
        vector<thread*> threads;
        string lock_type;
        string bar_type;
        int NUM_THREADS;
        int MAX;
        int split_size;
        vector <int> complete_data;
        vector<map<int, int> > buckets;

    public:
        BucketSort(int num_of_threads, string l_type, string b_type);
        // ~BucketSort();
        void MainDriver(vector <int> &data);
        void Bucket(int tid, Locks *lk, Bar* bar);
};