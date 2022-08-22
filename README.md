# PrimeFinder
This is a multithreaded program to find the amount of prime numbers up to a give number<br>
# Run the Program
Install repository:
```
git clone https://github.com/brettherbst/PrimeFinder.git
```
Run Program: <br>
```
$ make
$ ./findPrimes
```
By default, the program runs on 1 thread, and counts the amount of primes up to 100,000,000 <br>
```
$ ./findPrimes            
Thread 0 From 2 to 100000000: 5761455
---------------------------------------------------
Total number of primes less than 100000000: 5761455
Time measured: 87.2223 seconds
```
However, you can decrease your runtime significantly by running multiple threads: 
```
$ ./findPrimes -t 8
Thread 0 From 2 to 12499999: 818703
Thread 1 From 12500000 to 24999999: 747224
Thread 2 From 25000000 to 37499999: 724423
Thread 3 From 37500000 to 49999999: 710784
Thread 4 From 50000000 to 62499999: 700353
Thread 5 From 62500000 to 74999999: 692817
Thread 6 From 75000000 to 87499999: 686159
Thread 7 From 87500000 to 100000000: 680992
---------------------------------------------------
Total number of primes less than 100000000: 5761455
Time measured: 17.2928 seconds
```
You can also specify what number to calculate the primes up to (Max 18,446,744,073,709,551,615)
```
$ ./findPrimes -t 8 -n 4294967300
Thread 0 From 2 to 536870911: 26207278
Thread 1 From 536870912 to 1073741823: 25552386
Thread 2 From 1073741824 to 1610612735: 25145151
Thread 3 From 1610612736 to 2147483647: 24851777
Thread 4 From 2147483648 to 2684354559: 24621174
Thread 5 From 2684354560 to 3221225471: 24621174
Thread 6 From 3221225472 to 3758096383: 24435580
Thread 7 From 3758096384 to 4294967300: 24274125
---------------------------------------------------
Total number of primes less than 4294967300: 203280221
Time measured: 4943.14 seconds
```

Special thanks to [OpenCSF](https://w3.cs.jmu.edu/kirkpams/OpenCSF/Books/csf/html/) for inspiration of project idea and guidance on multithreading fundamentals