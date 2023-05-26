#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector <int> read_from_file(string file_name);
void write_to_file(string file_name, vector<int> data_to_write);
void print(std::vector <int> const &a);
void print_usage();
