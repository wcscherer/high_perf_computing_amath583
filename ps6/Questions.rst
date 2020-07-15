
Questions
=========

Norm
----

* Look through the code for ``run()`` in ``norm_utils.hpp``.  How are we setting the number of threads for OpenMP to use?
The 'run' function has a #if statement checking if _OPENMP exists. If it does, the for loop sets the number of threads to use for a given run
by utilizing the omp_set_num_threads(nthreads) function.  The number of threads are incremented by 2 and terminated by the total number of partitions.
If OPENMPM isn't installed, the program is run sequentially.


* Which version of ``norm`` provides the best parallel performance?  How do the results compare to the parallelized versions of ``norm`` from ps5?
The best avg. parallel performance I achieved was with norm_block_reduction() using 4 threads: 4.77 GFLOPS/s (running on 2 core machine). With 4 threads, the GFLOPS/s
ranged from ~4.73 for N=1048576 to ~4.76 for N=33554432.  The best peak performance of 4.89 GFLOPS/s was achieved with norm_block_reduction using 4 threads and
N = 16777216.  norm_parfor() achieved similar performance, although <10% slower on average. norm_parfor() was more consistent with it's results for each execution, but norm_block_reduction() still has
higher average performance. The first run of norm_block_reduction() is always slower than the second run. Not sure why, maybe it has to do with loading cache or expecting the cpu instructions. For 4 threads, I see a performance increase of ~1.25x over sequential when using norm_block_reduction().  In homework 5, I achieved around 1.5x speedup for pnorm and around 1.2x speedup
for fnorm.  My speedup results for norm_block_reduction() are on the same order of magnitude as my fnorm results, but lower than pnorm. It should be noted, that the results from PS5 were for 
smaller problem sizes. It is difficult to compare the two approaches directly since the OMP implementations are able to handle much larger problem sizes.


* Which version of ``norm`` provides the best parallel performance for larger problems (i.e., problems at the top end of the default sizes in the drivers or larger)?  How do the results compare to the parallelized versions of ``norm`` from ps5?
For N=33554432 and higher, norm_parfor seems to perform more consitently.  The maximum increase in performance over norm_block_reduction() isn't extremely large (< 10%) but norm_parfor() performs more consitently for large N.
norm_block_reduction() will sometimes drop in performance by over 100% for a given thread count when N is increased ( say go from 4.4 GFLOPS/s to 2.5 GFLOPS/s then go back to ~4 GFLOPS/s).  Max perforamnce between norm_block_reduction()
and norm_parfor() is hard to judge since norm_block_reduction() performance varies so much between executions.

* Which version of ``norm`` provides the best parallel performance for small problems (i.e., problems smller than the low end of the default sizes in the drivers)?  How do the results compare to the parallelized versions of ``norm`` from ps5?  
Both norm_block_reduction() and norm_parfor() have similar performance (within <10% difference) for N =512 and N= 1024.  Both cycliclcal implementations are worse compared to norm_parfor() and norm_block_reduction() except for
8 threads. For some reason, the cyclical implementations achieve about 2x performance compared to norm_parfor() and norm_block_reduction() when using 8 threads. 

Sparse Matrix-Vector Product
----------------------------

* How does ``pmatvec.cpp`` set the number of OpenMP threads to use?
pmatvec.cpp first checks if openmp is defined and available.  In the main{} function there is a for loop that defines the number of threads to iterate through.
If openmp is available, the omp_set_num_threads() function is called to set the number of threads to use defined by the loop. If openmp isn't installed,
the thread number is ignored and the program runs sequentially.

* (For discussion on Piazza.)
What characteristics of a matrix would make it more or less likely to exhibit an error 
if improperly parallelized?  Meaning, if, say, you parallelized ``CSCMatrix::matvec`` with just basic  columnwise partitioning -- there would be potential races with the same locations in ``y`` being read and written by multiple threads.  But what characteristics of the matrix give rise to that kind of problem?  Are there ways to maybe work around / fix that if we knew some things in advance about the (sparse) matrix?

* Which methods did you parallelize?  What directives did you use?  How much parallel speedup did you see for 1, 2, 4, and 8 threads?
I was able to successfully paralelize CSC::t_matve and CSR::matvec.  Both of these implmenetations allow for partitioning of y and therefore accomodate blocking. I was able to implement similar parallelization schemes
for both methods using an outer parallelization block call utilizing 'parallel for schedule(static)'. By calling schedule(static) the loops are divided into equal-sized chunks automatically without explicitly defining blocks. I also
employed hoisting on the y(i) variable, this also added some performance improvement.  
Results for parallelization:

Parallelized CSR::matvec relative to sequential CSR::matvec
 1 thread:  avg speedup of ~1.18x, peak of 1.26x for N=64 
 2 threads: avg speedup of ~1.59x, peak of 2.16x for N=64
 4 threads: avg speedup of ~1.67x, peak of 2.24x for N=128
 8 threads: avg speedup of ~1.39x, peak of 1.61x for N=128

Parallelized CSC::t_matvec relative to sequential CSC::matvec
 1 thread:  avg speedup of ~1.10x, peak of 1.20x for N=512
 2 threads: avg speedup of ~1.68x, peak of 2.19x for N=64
 4 threads: avg speedup of ~1.74x, peak of 2.52x for N=64
 8 threads: avg speedup of ~1.34x, peak of 1.70x for N=128

Sparse Matrix Dense Matrix Product (AMATH583 Only)
--------------------------------------------------

* Which methods did you parallelize?  What directives did you use?  How much parallel speedup did you see for 1, 2, 4, and 8 threads?  How does the parallel speedup compare to sparse matrix by vector product?
I parallelized matmat for both CSR and CSC.  I used an outer parallelization block call utilizing 'parallel for schedule(static)' for both methods. No hoisting was employed. Matmat for both CSC and CSR use the same
parallelization scheme as the schedule(static) call creates its own blocking procedures and makes copies of each iterator variable so the same iterator isn't accessed for each thread. Parallel performance characteristics
are measured relative to sequential performance where both methods use NRHS=10:

Parallelized CSR::matmat relative to sequential CSR::matmat
 1 thread:  avg speedup of ~0.88x, peak of 0.92x for N(Grid)=1024
 2 threads: avg speedup of ~1.69x, peak of 1.75x for N(Grid)=128
 4 threads: avg speedup of ~1.91x, peak of 1.98x for N(Grid)=256
 8 threads: avg speedup of ~1.90x, peak of 1.97x for N(Grid)=128

 Parallelized CSC::matmat relative to sequential CSC::matmat
 1 thread:  avg speedup of ~0.97x, peak of 1.10x for N(Grid)=128
 2 threads: avg speedup of ~1.84x, peak of 2.11x for N(Grid)=64
 4 threads: avg speedup of ~2.03x, peak of 2.36x for N(Grid)=128
 8 threads: avg speedup of ~1.93x, peak of 2.36x for N(Grid)=128

Single thread parallelization seems to be slightly slower than normal sequential due to thread initialization overhead.

PageRank Reprise
----------------

* Describe any changes you made to pagerank.cpp to get parallel speedup.  How much parallel speedup did you get for 1, 2, 4, and 8 threads?
I switched the CSR matrix to CSC in pagerank.cpp as was done in PS5a and utilized the pmatvec operation.  However, I keep getting errors generated from
pagerank_test that say my CSC matrix isn't behaving properly. This is confusing because my CSC class passed all the parallelized tests.  I'm not sure what that issue is and therefore I cannot use any of my results. I attempted to find
a way to parallelize the main for loop in pagerank.hpp, but I could not figure out how to handle the return statement. I personally felt that the lectures were not very helpful
in understanding implementing OMP functions. 


* (EC) Which functions did you parallelize?  How much additional speedup did you achieve?
In pagerank.cpp, I added #pragma omp parallel for schedule(static) to all of the for loops without a return call.  In my runs for Erdos02_nodename.txt,
I didn't see much improvement in the results. The biggest effect on performance is using the parallelized implementation of CSC::matvec, but I couldn't get that working.
Parallelizing the other loops didn't seem to add much performance, as for some of these loops the elapsed time was already sub 1 milisecond, so the timers wouldn't register
any improvement. 

Load Balanced Partitioning with OpenMP (Extra Credit)
-----------------------------------------------------

* Are there any choices for scheduling that make an improvement in the parallel performance (most importantly, scalability) of pagerank?