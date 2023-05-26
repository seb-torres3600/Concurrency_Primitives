CXX = g++
CXXFLAGS = -pthread -O3 -std=c++2a -g -Wall -Wextra

all: main

clean:
	rm read_write.o counter.o bar.o locks.o sort.o counter mysort

main: read_write.o bar.o locks.o counter.o sort.o counter_driver.cpp
	$(CXX) $(CXXFLAGS) counter_driver.cpp read_write.o bar.o locks.o counter.o -o counter
	$(CXX) $(CXXFLAGS) mysort_driver.cpp read_write.o bar.o locks.o sort.o -o mysort

read_write.o: read_write.cpp
	$(CXX) $(CXXFLAGS) -c read_write.cpp

bar.o: bar.cpp
	$(CXX) $(CXXFLAGS) -c bar.cpp

locks.o: locks.cpp
	$(CXX) $(CXXFLAGS) -c locks.cpp

counter.o: counter.cpp
	$(CXX) $(CXXFLAGS) -c counter.cpp

sort.o: sort.cpp
	$(CXX) $(CXXFLAGS) -c sort.cpp