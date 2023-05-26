#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "read_write.hpp"

using namespace std;

 // https://cplusplus.com/doc/tutorial/files/
vector <int> read_from_file(string file_name){
    vector <int> data; // vector holding our dat
    string line;
    ifstream myfile(file_name); // stream class to read from file
    int num;
    if (myfile.is_open()) // if file is open 
    {
        while ( getline (myfile,line) )
        {   
            try{
                num = stoi(line); //convert string line to an integer
                data.push_back(num); // add the number to our data vector
            }
            catch(invalid_argument &){ // had the issue where the final whitespace was causing an invalid_argument error
                // https://github.com/Haivision/srt/issues/568
                continue;
            }
        }
        myfile.close(); 
    }
    return data; // return the data vector
}

void write_to_file(string file_name, vector<int> data_to_write){
    ofstream file; //stream class to open file
    file.open(file_name); //opemn file
 
    for(int i = 0;i < int(data_to_write.size()); i++)
    {
        file << data_to_write[i]<<endl; // write data to file
    }
 
    file.close();
}

// helper to print vector for debugging
void print(std::vector <int> const &a) {
   for(int i=0; i < int(a.size()); i++){
    cout << a.at(i) << " ";
   }
}

// print usage helper function
void print_usage(){
    cout << " Invalid barrier type\nUsage: counter [--name] [-t NUM_THREADS] [-i NUM_ITERATIONS] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,pthread>] [-o outfile.txt]" << endl;
}
