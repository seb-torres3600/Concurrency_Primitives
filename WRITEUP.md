# Lab 2 Write Up - Sebastian Torres

## Summary: 
The goal of this lab was to create our own primitives with atoomic variables and operations. 
We had to create 3 locks and 1 barrier algorithm. The locks are test and set, test test and set, 
and ticket. The only barrier implemented in sense wait. There are two executables that get 
created by the Makefile, mysort and counter. Details about algorithms, performance, file organizaton,
and description of files are all listed below. 

## Algorithms:

**Test and Set** - This primitive uses an atomic "flag" boolean 
variable and atomically checks if it is false. If it is false, then it is changed to true,
if it is not then it will spin waiting in the while loop acting as a lock waiting for the lock
to become false. This was implemented with the atomic compare and exchange operation, I opted for
strong becuase the lecture notes state that weak can often fail unexpectedly.

**Test Test and Set Lock** - Similar to the Test and Set
lock above, this performs a compare and exchange but also checks if checks if the boolean
variable "flag" is set to true. It does this to reduce the amount of cache line invalidations
occurring while the lock is trying to be acquired. Test and Set is a simple lock to implement
but it's scalability is not great so this is much better. 

**Ticket** - This primitive is well described by the name of it. It acts like a 
ticketing system at the DMV or market. It uses two atomic variables, similar to
the lecture slides, they are named "next number" and "now serving". The lock holder
is the thread where the local "my number" is equal to what is being served. In order 
to unlock this we need to do a fetch and add, which will give the lock to the next waiting
thread.

**Sense** - This is a simple barrier algorithm that determines when all threads have arrived 
at the barrier. It does this by giving each thread a local boolean "my sense" variable that 
gets flipped once the thread arrives. Once the number of "my sense" variables has reached the
amount of threads that there are then the barrier releases the threads to continue doing 
work. I strictly used atomic operation in my barrier as opposed to the lock version. Both were 
show in the lecture slides.

**Bucket Sort** - Copied from Lab 1

**Counter** - Copied from Lab 2 Description. There are two functions, one that gets
ran with locks and the other with barriers. The lock implementation does end up using a 
"pthread" barrier at the end before returning because the program was failing as the main thread
would return quicker than others and try to join threads that had not complete their work.

**Pthread** -  any pthreads implementation calls the proper C++ mutex or barrier as 
in lab 1

## Performance:
**The averages of 3 runs**

./counter -t 4 -i 10000 --lock=<tas,ttas,ticket,pthread> -o out.txt  

| Primitive | Runtime(s) | L1 cache hit(%) | Branch Pred Hit Rate(%) | Page Fault Count(#) |  
|-----------|------------|-----------------|-------------------------|---------------------|
| TAS       | 0.0134264  | 92.118          | 98.896                  | 146                 |
| TTAS      | 0.0307387  | 97.932          | 99.40                   | 145                 |
| Ticket    | 0.0313704  | 98.908          | 99.74                   | 147                 |
| PThread   | 0.0219446  | 96.682          | 98.782                  | 145                 |

./counter -t 4 -i 10000 --bar=<sense,pthread> -o out.txt

| Primitive | Runtime(s) | L1 cache hit(%) | Branch Pred Hit Rate(%) | Page Fault Count(#) |  
|-----------|------------|-----------------|-------------------------|---------------------|
| sense     | 0.03742728 | 99.113          | 97.83                   | 149                 |
| pthread   | 0.22236301 | 97.056          | 97.816                  | 146                 |


./mysort ./autograde_tests/550000in1-1000000skew.txt -o out.txt -t 4 --lock=<tas,ttas,ticket,pthread> --bar=pthread

| Primitive | Runtime(s) | L1 cache hit(%) | Branch Pred Hit Rate(%) | Page Fault Count(#) |  
|-----------|------------|-----------------|-------------------------|---------------------|
| TAS       | 1.37187    | 96.798          | 98.076                  | 9051                |
| TTAS      | 1.21810    | 98.748          | 98.748                  | 9060                |
| Ticket    | 1.28145    | 99.06           | 99.493                  | 9060                |
| PThread   | 1.55178    | 96.636          | 97.98                   | 9057                |

./mysort ./autograde_tests/550000in1-1000000skew.txt -o out.txt -t 4 --lock=<tas,ttas,ticket,pthread> --bar=sense

| Primitive | Runtime(s) | L1 cache hit(%) | Branch Pred Hit Rate(%) | Page Fault Count(#) |  
|-----------|------------|-----------------|-------------------------|---------------------|
| TAS       | 1.4937725  | 96.88           | 98.043                  | 9056                |
| TTAS      | 1.2234955  | 98.95           | 99.286                  | 9060                |
| Ticket    | 1.2730985  | 99.06           | 99.48                   | 9062                |
| PThread   | 1.6046843  | 97.65           | 98.44                   | 9060                |

I gathered all my data utilizing perf. My sense barrier algorithm did much better than the 
regular pthread one. I think it has to do with the fact that I strictly used atomic operations
rather than a lock. All my evaluations ended up coming really close to each other. But the ticket
lock seems to be the best locking algorithm in both scenarios. I think this has to do with fairness
in the bucket sort part but I'm not sure why that is the case in the counter scenario.  

## Challenges:
The largest challenge as with every multi-threaded program is the debugging. I sometimes
saw error that I had never seen before. It took a lot of googling to figure out what was 
going on. Luckily the lecture slides provided a great starting point wiht the algorithms. 

## Code Organization:

I did my best to organize this lab so it was easy to go through. All my barriers are placed
in "bar" files. Locks are placed in "locks" files. My counter algorithms are in "counter" files
while the main counter driver that takes in the arguments is called "counter_driver". I have 
my bucket sort implemented in "sort" files. It has mostly been copied from Lab 1. The main
driver for that is "mysort_driver". My helper file to read and write data to a document is placed in
"read_write" files.

bar.cpp/bar.hpp - barrier implementations
locks.cpp/locks.hpp - lock implementations
counter.cpp/counter.hpp - counter implementations
counter_driver.cpp - driver file for counter executable
read_write.cpp/read_write.hpp - helper functions to read and write to files
sort.cpp/sort.hpp - bucket sort implementation copied mostly from lab 1
mysort_driver.cpp - driver file for my_sort executable


## Description of Files:
bar.cpp - this file consists of 4 items. We have a constructor that takes in the 
barrier type we would like to use. It initializes the proper variables depending on what
on the barrier type. We have a destructor which is used when we call a pthread barrier 
in order to delete that barrier. We have our sense_wait algorithm that is described above
and a driver function that is what gets called. The driver function calls the sense_wait algo
or the regular arrive_and_wait. 

bar.hpp - header file for bar.cpp

locks.cpp - This file consists of multiple functions. We have our constructor that initializes
variables depending on the info given to it. The driver functions are named acquire and release.
They call the appropriate lock or unlock algorithms depending on the lock type given. 

locks.hpp - header file for locks.cpp

counter.cpp - file that has the counter lock and counter barrier implementations. It has a main
driver function that gets called and then it determines the appropriate counter function to call.

counter.hpp - header file for counter.hpp

read_write.cpp - helper file to read and write data from files, same as lab 1

read_write.hpp - header file for read_write.cpp

sort.cpp - this includes my bucket sort implementation from lab 1. 

sort.hpp - header file for sort.cpp

counter_driver.cpp or mysort.cpp - main driver files for my executables. Takes in
command line arguments to determine what function calls it needs to make. 

## Bugs:

None at this time. 


