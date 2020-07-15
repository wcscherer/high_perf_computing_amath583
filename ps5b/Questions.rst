
pnorm
-----

* What was the data race?
Every thread was accessing and updating sum.  Different threads could be accessing sum at an intermediate step before
the previous thread completed its full exectuion, so the current thread is building off of an incorrect partial sum. 

* What did you do to fix the data race?  Explain why the race is actually eliminated (rather than, say, just made less likely).
In the helper function worker_a, I added a variable pts and modified the partial sum in the loop to update pts, not partial. 
Then, outside of the loop, I updated the reference to partial (sum) by adding the new partial sum variable pts.  Pulling
the update to partial out of the loop prevents other threads from summing into partial (sum) before another thread is complete.
A mutex lock is put around the update of partial sum to ensure every thread updates it properly.  Partial is only updated with
the current partial sum after the partial sum is complete. It doesn't matter in what order the partial sums are summed into partial,
just that all threads contribute their partial sum at the end. 

* How much parallel speedup do you see for 1, 2, 4, and 8 threads?
1 thread: 1.0x speedup, 2 threads: ~1.3x speedup, 4 threads: ~1.5x speedup, 8 threads: ~1.5x speedup
Overall, after 4 threads, speedup never excedds ~50% faster. My machine has two physical cores, so it makes sense that
the ceiling on the improved performance appears to be 2x. 

fnorm
-----

* How much parallel speedup do you see for 1, 2, 4, and 8 threads for ``partitioned_two_norm_a``?
1 thread: ~0.92x speedup, 2 threads: ~1.18x speedup, 4 threads: ~1.17x speedup, 8 threads: ~1.15x speedup
Overall, after 2 threads, speedup never excedds ~20% faster.

* How much parallel speedup do you see for 1, 2, 4, and 8 threads for ``partitioned_two_norm_b``?  
1 thread: ~1.04x speedup, 2 threads: ~1.03x speedup, 4 threads: ~1.03x speedup, 8 threads: ~1.02x speedup
Overall, speedup never excedds 3% faster.

* Explain the differences you see between ``partitioned_two_norm_a`` and ``partitioned_two_norm_b``.
For partitioned_two_norm_a, there is actual speedup, ~20%, which is inline with the pnorm method it is
based off of.  For partitioned_norm_b, there isn't any significant speedup compared to the sequential run. 
By running "deferred", two_norm_b essentially operated in serial mode, made evident by no speedup.  Allowing
for asynchrous processing of the worer function with "async", some speedup was achieved. 

cnorm
-----

* How much parallel speedup do you see for 1, 2, 4, and 8 threads?
I do not see any speedup, but slowdown compared to sequential performce for all thread implementations. I tried both a futures implementation
similar to fnorm and a threaded implementation similar to pnorm.  I am submitting my threaded implementation since I achieved slightly better performance than 
my futures implementation.  Both implementations passed cnorm_test.exe, but I understand that does not mean the code is correct.  I see no speedup for any
number of threads considered. I actually see slowdown for all numbers of threads considered relative to sequential performance. 

1 thread: ~0.5x speedup, 2 threads: ~0.8x speedup, 4 threads: ~0.6x speedup, 8 threads: ~0.3x speedup

* How does the performance of cyclic partitioning compare to blocked?  Explain any significant differences, referring to, say, performance models or CPU architectural models.
Cyclic performance is significantly lower than the pnorm and fnorm blocked implementations. Cnorm offered no speedup, whereas both threaded and futures implementations
offered significant speedup ~1.5x and ~1.2x respectively.  I believe this has to do with how the arrays are loaded and accessed. For cnorm, the array locations being accessed 
may jump all over the place depending on the number of threads (partitions).  These values may not be in cache so for cnorm the cpu may have to fetch more values from higher level caches
or even RAM, which would significantly slow down the operation.  Both pnorm and fnorm sum over continuous subsections of an array, most if not all of which can be loaded into cache
since the accessed values are next to each other. 

rnorm
-----

* How much parallel speedup do you see for 1, 2, 4, and 8 threads?
I do not know, I didn't do this question. I didn't have time to figure this out too.

* What will happen if you use ``std:::launch::deferred`` instead of ``std:::launch::async`` when launching tasks?  When will the computations happen?  Will you see any speedup?  For your convenience, the driver program will also call ``recursive_two_norm_b`` -- which you can implement as a copy of ``recursive_two_norm_a`` but with the launch policy changed.
It'll run slower. 

General
-------

* For the different approaches to parallelization, were there any major differences in how much parallel speedup that you saw?
The best speedup I achieved was >1.5x for pnorm using locked threads, which decent for an old 2-core cpu. My solution for fnorm using futures offered some speedup ~1.2x My cnorm implementations using futures and threads was significantly slower than sequential.
Cnorm with 2 threads was ~0.80x that of the sequential performance.  1 thread for cnorm was about 0.5x and 4-8 threads were even worse. 


* You may have seen the speedup slowing down as the problem sizes got larger -- if you didn't keep trying larger problem sizes.  What is limiting parallel speedup for two_norm (regardless of approach)?  What would determine the problem sizes where you should see ideal speedup?  (Hint: Roofline model.)
The worst speedup I got (for an implementation that worked) is slightly less than 1x due to process management. If the problem is so large that most of the values can't be loaded into cache, then multiple threads aren't going to help because the speed
of the algorithm depends heavily on loading data into cache, which is expensive. Threads cannot load in data faster, only execute on what data is there. 

Conundrum #1
------------

1. What is causing this behavior?
I see extremely high performance (~10 GFLOPS/s) for the sequential execution of pnorm for 256-512 sizes.  For implementing 1-8 threads, the performance was extremely slow, on the order of 0.01 GFLOPS/s.
The main difference between the two implementations is the use of threads and locks.  For a small problem, the locks and thread creation take up a larger portion of execution time compared to larger problems.
This switching between locked and unlocked as well as rapidly changing threads slows the performance down considerably. 


2. How could this behavior be fixed?
Write a wrapper function that estimates problem run tim ebased on input size then assign parallelized or serial solution respectively. 


3. Is there a simple implementation for this fix?
Probably, but I can't think of any.


Parallel matvec
---------------

* Which methods did you implement?

I implemented parallel CSC matvec and t_matvec and CSR matvec and t_matvec.  For parallelization, I used a helper function
to compute the thread partial matvec operation and a main matvec function that assigns partitions of the matrix and vector 
to a thread.

* How much parallel speedup do you see for the methods that you implemented for 1, 2, 4, and 8 threads?
For all four implementations, I see slow performance (~0.4x speedup) for N = 64. At 128, some performance increase begins to appear. 
At N(matrix) = 256, I see roughly 1x speedup for 1 thread compared to unmodified COO implementations. Above N=256, the
parallelized implementations achieve > 1x speedup compared to unmodified COO for all thread numbers. 
For 1 thread, I see a max speedup relative to COO of ~1.5x (CSC^T N=2048) and a min speedup of ~1.1x (CSR N=128)

For 2 threads, I see a max speedup relative to COO of ~1.9x (CSR^T N=2048) and a min speedup of ~1.3x for (CSR^T N=128)

For 4 threads, I see a max speedup relative to COO of ~2.0x (CSR^T N=1024) and a min speedup of ~1.0x for (CSC N=128)

For 8 threads, I see a max speedup relative to COO of ~1.9x (CSC^T N=1024) and a min speedup of ~0.4x for (CSR^T N=128)

Conundrum #2
------------

1. What are the two "matrix vector" operations that we could use?
You could use the CSC implementation for matvec instead of CSR and include the multithreaded option.  Or write a t_matvec operation for CSR that actually works.



2. How would we use the first in pagerank?  I.e., what would we have to do differently in the rest of pagerank.cpp to use that first operation?
Change the call to CSRMatrix in pagerank to CSCMatrix, and adjust the unit tests accordingly so they don't fail on pagerank trying to test CSR functions.

3. How would we use the second?
Write a t_matvec operation for CSR  that is parallelized properly and rewrite the calls in pagerank to CSR transpose. 