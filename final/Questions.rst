

Final Questions
===============

Norm
----


* Per our discussions in lectures past about weak vs strong scaling, do
  the plots look like what you would expect? Describe any (significant)
  differences (if any).

Strong scaling: increase the number of processors while holding the problem size the same
Weak scaling: problem size assigned to each processor stays the same, but more processors are added for larger total problem sizes. 
The strong scaling plot looks as expected for strong size > 25, as the number of processors increases the GFLOPS/s increases. This makes sense because
the problem size stays the same but with more prcessors, more operations are being performed per second and the problem executes faster - as expected. 
The weak scaling plot also looks as expected. For weak scaling, you expect the exectution time to stay roughly constant (~1x speedup) as the number of processors
increases and the problem size increases.  If the problem size is held constant, there should be a cutoff where adding more processors slows down exectution time
because of the time management overhead for coordinating the additional processors.  For the weak scaling plot, problem size of 22 has approximately 1x speedup for adding 
an additional processor up to 32 processors. For weak20, the speedup provided by each additional processor decreases from 1 to 32 processes. 

* For strong scaling, at what problem size (and what number of nodes) does
  parallelization stop being useful?  Explain.

For strong scaling, after about size 25 (strong25) there is no significant increase in speedup for larger problem sizes.  For strong20, the number of GFLOPS/s decreases after adding 8 processes.
For all the problem sizes less than 25, there is little to no significant increase in speedup after adding 8 processes. The decrease in performance is likely do to the problem not being large enough for 
additional processors to significantly speed up the solution due to the additional overhead required.

Grid
----

* Per our discussions in lectures past about weak vs strong scaling, do
  the plots look like what you would expect? Describe any (significant)
  differences (if any).

  Both the weak and strong scaling plots look as expected.  For large problem sizes (512), the weak scaling plot indicates that the speedup provided by an additional processor decreases less for large problem sizes (512)
  than for the smaller problem sizes (128).  This is due to the overhead of adding an additional process taking up more time to manage than the speedup provided by passing part of the problem to an additional processor.
  For the strong scaling plot, as the problem size gets larger, the speedup increases for each additional process.  For N=4096, speedup is achieved with every additional processor added, whereas for N=1024 adn N = 2048, there
  is a cutoff where additional processors do not significantly speed up the calculation.

* For strong scaling, at what problem size (and what number of nodes) does
  parallelization stop being useful?

  For N = 1024, adding additional processes beyond 4 seems to be unnecessary as no significant additional speedup is achieved by adding an additional process.
  For N = 2048, adding additional processes beyond 16 seems to be unnecessary as significantly less additional speedup is achieved by adding an additional process.
  For N = 4096, adding an additional process increased the performance approximately the same amount all the way to the maximum amount of processes was reached (32).
