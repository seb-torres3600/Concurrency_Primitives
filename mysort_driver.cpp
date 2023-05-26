/*
Sebastian Torres
09.16.2022
Lab 2
*/

#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include "read_write.hpp"
#include "sort.hpp"

using namespace std;

int main(int argc, char** argv){

    // Variable declarations
    int our_option;
    // Num of threads defaults to 4 if nothing passed
    int  NUM_THREADS = 4;
    string barrierType = "pthread"; // default as pthread if none given
    string lockType = "pthread"; //default to pthread if none given
    string counter_type;
    string outputFile;
    string sourceFile;

    //get source file
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
        our_option = getopt_long(argc, argv, "nb:l:t:i:o:",long_options, &index);

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

                // If invalid barrier type input is entered
                if(barrierType != "sense" && barrierType != "pthread" ) {
                    print_usage();
                    return 1;
                }
                break;

            }

                // Choose lock type
            case 'l': {

                lockType = optarg;

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

                // Name of outputfile
            case 'o': {

                outputFile = optarg;
                break;

            }

        }

    }

    // vector to hold data
    vector <int> data;
    // read from source file and turn into vector
    data = read_from_file(sourceFile);
    //call sorter
    BucketSort sorter(NUM_THREADS, lockType, barrierType);
    sorter.MainDriver(data);
    // write to file
    write_to_file(outputFile, data);
    return 0;
}
