# DDD - Inter Process Communication with shared memory circular queue

Their are a small number of somewhat supported implementations on an inter-
process queue designed for a single-producer - single-consumer application.

Unid communicates to the application using a shared memory with the consumer
operating on one process thread, and the producer operating on a separate 
process thread.

## Design Objective

* Lock-free design
* Single producer, single consumer
* Fixed size queue
* Performant
* Well documented code
* No hardware dependencies
* Latest release (no use of a moving target)
* License compatible for our inclusion into a commercial product

It is our intention to test and benchmark the three top open source condenders
and then to select a winner that will be included into the source code of this
project.

When included, the responsibility for updating, bug fixing, refactoring as
needed to support new hardware and software technologies resides within the
unid project. NO RELIANCE ON OPEN SOURCE CODE MAINTENANCE!!

## Contenders

1. rigtorp/SPSCQueue (C++11): A popular header-only, lock-free queue that works
with non-power-of-two capacities.
1. cameron314/readerwriterqueue (C++): Highly optimized, often used in high-
performance trading software, and supports cache line padding.
1. ihebu/spscq: Another lock-free, cache-optimized, and header-only library
that specifically focuses on single-producer/single-consumer use cases. It aims
to prevent false sharing between CPU cores, which is a common performance
pitfall in concurrent programming.
1. seancfoley/High-Performance-CPP-Queues: This repository contains queue
implementations that minimize or avoid memory allocations during critical
operations by using assignment copying. It is designed to be fast and uses
basic wrappers for mutexes and condition variables. 

## Results

### Benchmarks

All of the contenders are tested with the same benchmark test suite. The
test suite is implemented using the Catch2 unit test library. All of the
tests are performed with the same compiler and hardware environment. All
of the tests are performed multiple times, allowing standard deviation,
best, worst and average performance metrics.

### [rigtorp/SPSQueue](https://github.com/rigtorp/SPSCQueue)

### [cameron314/readerwriterqueue](https://github.com/cameron314/readerwriterqueue)

### [ihebu/spscq](https://github.com/ihebu/spscq)

### Conclusion
