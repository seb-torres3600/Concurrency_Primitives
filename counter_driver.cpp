/*
Sebastian Torres
09.16.2022
Lab 2
*/

#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include "read_write.hpp"
#include "counter.hpp"

using namespace std;

int main(int argc, char** argv){

    // Variable declarations
    int our_option;
    // Num of threads defaults to 4 if nothing passed
    int  NUM_THREADS = 4;
    int NUM_ITERATIONS = 0;
    string barrierType = "none";
    string lockType = "none";
    string bar_or_lock = "none";
    string counter_type;
    string outputFile;
    string sourceFile;

    // if not asking for name, first argument is source file
    if (argc > 2){
        sourceFile = string(argv[1]);
    }

    // Creating the long options
    static struct option long_options[] = {
        {"name",   no_argument,         0,  'n' },
		{"bar",   required_argument,   0,  'b' },
		{"lock",   required_argument,   0,  'l' },
        {0, 0, 0, 0}
    };

    // get all arguments
    while(1) {

        int index = 0;
        our_option = getopt_long(argc, argv, "nb:l:t:i:o:", long_options, &index);

        // When all the options have been read
        if (our_option == -1) {
            break;
        }

        switch(our_option) {

            // Name option. Program will terminate after printing name.
            case 'n': {

                cout << "Sebastian Torres" << endl;
                return 0;

            }

                // Choose barrier type
            case 'b': {

                barrierType = optarg;
                bar_or_lock = "bar";

                // If invalid barrier type input is entered
                if(barrierType != "sense" && barrierType != "pthread") {
                    print_usage();
                    return 1;
                }
                break;

            }

                // Choose lock type
            case 'l': {

                lockType = optarg;
                bar_or_lock = "lock";

                // If invalid lock type input is entered
                if(lockType != "tas" && lockType != "ttas" && lockType != "ticket" && lockType != "mcs" && lockType != "pthread" && lockType != "petersonseq" && lockType != "petersonrel" ) {
                    print_usage();
                    return 1;
                }
                break;

            }

                // Number of threads
            case 't': {

                NUM_THREADS = atoi(optarg);
                if(NUM_THREADS > 150){
                    cout << "ERROR; too many threads\n" << endl;
                    return -1;
                }

                break;

            }

                // Number of iterations
            case 'i': {

                NUM_ITERATIONS = atoi(optarg);
                if(NUM_ITERATIONS <= 0){
                    cout << "Num iterations <= 0 " << endl;
                    return -1;
                }
                break;

            }

                // Name of outputfile
            case 'o': {

                outputFile = optarg;
                break;

            }

        }

    }

    // if both barrier and lock type chosen, not allowed
    if (barrierType != "none" && lockType != "none" ){
        cout << "You may not choose both barrier and lock type" << endl;
        cout << "Please only pick one" << endl;
        return -1;
    }

    // if none are picked
    if(barrierType == "none" && lockType == "none"){
        cout << "Must pick a barrier or locktype" << endl;
        return -1;
    }

    vector <int> data;
    int count;
    // if we are using barriers
    if(bar_or_lock == "bar"){
        Counter counter(barrierType, NUM_THREADS, NUM_ITERATIONS, bar_or_lock);
        count = counter.counter_driver();
    }
    // if locks are being used
    else if(bar_or_lock == "lock"){
        Counter counter(lockType, NUM_THREADS, NUM_ITERATIONS, bar_or_lock);
        count = counter.counter_driver();
    }
    else{
        cout << "Error: No barrier or lock" << endl;
        return -1;
    }

    // created it into vector because I already helper function to write vector to file
    data.push_back(count);
    write_to_file(outputFile, data);
    return 0;
}
