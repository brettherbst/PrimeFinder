#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include <chrono>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;




//Number of threads Running
//(This number should ideally be the number of cores your CPU has)
int NUM_THREADS = 1;
// Each thread deals with a range of numbers, and count is the number of primes in that range
struct threadRange {
  uint64_t start;
  uint64_t end;
  uint64_t count;
};

// Function that the threads use to count the number of primes
void * getPrimeRange (void * _ranges) {
    // Cast the ranges to a our usabel struct type 
    struct threadRange *ranges = (struct threadRange*) _ranges;

    // Skip over any numbers < 2 (2 is the smallest prime)
    if (ranges->start < 2) ranges->start = 2;

    // Loop through the current thread's  start to end 
    ranges->count = 0;
    for (uint64_t i = ranges->start; i <= ranges->end; i++){
        //Start with j = 2; see if j divides the given number i evenly. If it does, i is not prime
        //If we do this until we reach sqrt(i), and is_prime is still true, then the number i must be prime
        bool is_prime = true;
        for (uint64_t j = 2; j * j <= i && is_prime; j++){
            if (i % j == 0){
                is_prime = false;
                break;
            }
        }
        if (is_prime) ranges->count++;
    }

  //Exit once all primes are found
  pthread_exit (NULL);
}

int main (int argc, char **argv) {
    //number to cout primes up to, 100,000,000 by default
    uint64_t number_count = 100000000;
    /* =====================================
       Parse argv if arguments are given
       =====================================
    */
    bool invalidArgument = false;
    string badArg;
    string helpInfo = "To run the Program: ./findPrimes\nOptional Flags:\n -t: Number of threads to use\n -n: the number to find find primes up to(do not include commas)\nExample: ./findPrimes -t 8 -n 1000000\n(Find the number of primes up to 1,000,000 using 8 threads)\n(2<n<18,446,744,073,709,551,615)\n";
    if(argc > 1){
        for(int i=1;i< argc;i++){
            if(argv[i][0] != '-' || strlen(argv[i]) > 2){
                invalidArgument = true;
                badArg = argv[i];
                break;
            }else{
                switch (argv[i][1])
                {
                case 't': {
                    if(i+1 > argc){
                        cout << "You must specify a number of threads to use!\n";
                        cout << helpInfo;
                        return 1;
                    }
                    NUM_THREADS = stoi(argv[i+1]);
                    i++;
                    break;
                }
                case 'n': {
                    if(i+1 > argc){
                        cout << "You must specify a number of threads to use!\n";
                        cout << helpInfo;
                        return 1;
                    }
                    uint64_t num;
                    string numString = argv[i+1];
                    istringstream iss(numString);
                    if(iss >> num && num > 2){
                        number_count = num;
                    }else{
                        cout << numString << " is not a valid number!\n";
                        cout << helpInfo;
                        return 1;
                    }
                    i++;
                    break;
                }
                default:
                    invalidArgument = true;
                    badArg = argv[i];
                    i = argc+1;
                }
            }
        }
    }
    if(invalidArgument){
        cout << "Sorry, \"" <<  badArg  << "\" is not a valid argument\n";
        cout << helpInfo << endl;
        return 1;
    }
    if(NUM_THREADS > number_count){
        cout << "Sorry, number of threads (" << NUM_THREADS << ") cannot be bigger than the number being searched for (" << number_count << ")\n";
        return 1;
    }
    /* =====================================
       End of parsing argv arguments
       =====================================
    */

    //starting time for analytics 
    auto timerBegin = std::chrono::high_resolution_clock::now();

    pthread_t threads[NUM_THREADS];
    struct threadRange *ranges[NUM_THREADS];

    //divide number of primes per thread to count evenly, with the odd amount going to last thread
    uint64_t start = 0;
    uint64_t end = start + number_count;
    uint64_t number_per_thread = number_count / NUM_THREADS;


    // Assign the start/end values for each thread, then create/run the thread
    for (int i = 0; i < NUM_THREADS; i++){
        ranges[i] = (struct threadRange *) calloc (sizeof (struct threadRange), 1);
        ranges[i]->start = start + (i * number_per_thread);
        if(i+1 == NUM_THREADS){
            ranges[i]->end = number_count;
        }else{
            ranges[i]->end = ranges[i]->start + number_per_thread - 1;
        }
        assert(pthread_create (&threads[i], NULL, getPrimeRange,ranges[i]) == 0);
    }
    //Join threads and tally results
    uint64_t total_number = 0;
    for (int i = 0; i < NUM_THREADS; i++){
        pthread_join (threads[i], NULL);
        cout << "Thread " << i << " From " << ranges[i]->start << " to " << ranges[i]->end << ": " << ranges[i]->count << endl;
        total_number += ranges[i]->count;
        free (ranges[i]);
    }

    //end timer
    auto timerEnd = std::chrono::high_resolution_clock::now();
    //calculate how long it took to find the number of primes
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerBegin);

    // Display results and analytics
    cout << "---------------------------------------------------" << endl;
    cout << "Total number of primes less than " << end << ": " << total_number << endl;
    cout << "Time measured: " << elapsed.count() * 1e-9 << " seconds"<< endl;

    return 0;
}