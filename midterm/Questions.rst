To-Do 6) Is there any significant performance difference between CSC matvec and CSR matvec? If so, what's 
the advantage one implementation versus the other, if any?

CSC matvec seems to have a slight repeatable speed advantage over CSR matvec (around 5% higher GFlops/s).  Surprsingly,
CSC^T performs significantly faster than CSR (almost 25% faster through N grid = 512). This is surprising because CSC^T matvec
is esentially the same algorithm as CSR matvec- such a large repeatable speed difference for smaller matrices points to higher
efficiencies of CSC^T over CSR.  If standard CSC is a little faster than CSR, this might be explained by initializing a CSC matrix 
being faster than initializing a CSR matrix.  My implementation of CSC^T matvec is the exact same code for CSR matvec, so the only
difference seems to be the initialization of CSC matrices versus CSR. CSC loops through each column element in a row, which should
be faster than looping through each row element of a column (CSR) because matrix elements are loaded in by row, not column. 

1) How does the performance (in GFLOP/s) for sparse-matrix by vector
product compare to what you previously achieved for dense-matrix by
dense-matrix product?  Explain, and quantify if you can, (e.g., using
the roofline model).

The striking difference in performance between sparse-matrix by vector product versus dense-dense matrix product occurs for 
matrices with N above 256. From HW 4, standard ijk loop order dense matrix-matrix product achieved around 0.933 GFlops/s for N=256.
The highest performace of dense-dense matrix product (with out hoisting, tiling etc) was the ikj ordered matrix matrix producy. At 
N = 256, ikj loop ordering achieves ~4.9 GFlops/s and for N=512 ~4.4 GFlops/s. CSC^T (fastest) matrix-vector product achieved ~1.6 GFlops/s 
at N=256 and even achieved ~1.5 GFlops/s at N = 2048.  What I find most interesting is the difference in speed with which the results are 
printed between dense-dense product and sparse - vec product. Even the fastest dense-dense product takes a while to write results
for N = 512 (>30 seconds) where as sparse-vec product can write results for N=2048 in less than 8 seconds. The best dense-dense product achieves
a much higher GFlops/s performance than the best sparse-vec product for the same N - however, the sparse-vec product results are printed much faster.
This suggests that while the optimized dense-dense algorithms are better at doing more floating pointer operations per cycle, the sparse-vec
algorithms are better at performing fewer floating point operations in a much shorter time. This can partly explain why sparse-vec products 
methods have lower GFlops/s, but perform faster: they do significantly less floating point operations per-cycle in a relatively short amount of time.
None of the sparse-vec methods approach my computers roofline of around 13 GFlops/s. Sparse matrices are desgined for requiring less
floating point operations, so this observation appears to agree. The lower GFlops rate of the COO/CSR methods could also be partially explained by
more of the double numbers used in the operation not coming from cache. Since CPUs load in a whole row of a matrix at a time, without the looping through
the rows of A and columns of B, just bouning around to different B locations where there aren't non-zero numbers in A, the memory locations accessed for
B won't necessarily be right next to each other.  These locations in B may have to be pulled from RAM, so that would reduce the bandwidth and therefore GLOP rate. 
Assuming around 2 double operations for loop cycle, that gives a numeric intensity around 0.12, and at that numeric intensity, the GFlops/s for accessing RAM
is around 1.2 GFlops/s, so this assessment agrees with the Roofline model. 

2) How does the performance (in GFLOP/s) for sparse-matrix by vector
product for COO compare to CSR?  Explain, and quantify if you can,
(e.g., using the roofline model).

Between N=64 and N=2048, CSR mat-vec product performs about 1.4x-1.3x more GFlops/s compared to COO mat-vec product. Also, CSR^T mat-vec 
product performed around 1.3x-1.1x more GFlops/s than COO^T between N=64 and N=2048.  What is interesting is COO implemetation has more elements to iterate
through each loop than CSR, but only requires one loop. CSR requires 2 loops, but loops through fewer elements due to compressed row indexing.
Without explicit timing information for CSR vs COO for given N, it's hard to isolate the reason for CSR's higher GFlops rate.  Again, the reduction in
performance (GFlop rate). With fewer total numbers to index through, there should be less flops to perform and less memory locations to access. COO doesn't 
use any list compression, so it has to access more memory locations. If some of these values aren't in cache, they will take longer to access,
adding the the execution time and reducing Flop/s.  Since none of the Sparse matrix methods exceed even 2 Gflops.s, for a numeric intensity of around 0.1 (2 double ops)
this suggests that RAM is being accessed for all the compressed methods, but more data can be held in cache for CSR and there's less data to access, which helps improve performance
relative to COO.

3) How does the performance (in GFLOP/s) for sparse-matrix by dense
matrix product (**SPMM**) compare to sparse-matrix by vector product
(**SPMV**)? The performance for SPMM should be about the same as for
SPMV in the case of a 1 column dense matrix.  What is the trend with
increasing numbers of columns?  Explain, and quantify if you can,
using the roofline model.

For mat-vec, COO has a higher GFlop/s than COO mat-mat (about +20%), although COO mat-mat (for 1 column 'matrix') is around the 
same GFlop/s for all sizes N(Grid) ~0.55. CSR and CSC both perform between 3.5-2.8 GFlops/s for N=64 to N=2048 for multiplication
of a column vector this over a factor of 2 higher than CSR or CSC for mat-vec performance. This suggests that the mat-mat implementation
is able to load more data into cache than the mat-vec implementation at higher float/byte or does more flops per byte, since at 3.5 GFlops RAM is accessed
at about 2.2 floats/byte, where L3 cache at 3.5 GFlops maxes out at 0.1 FLOPs/byte.  If 2 flops/16bytes (0.125) is used as the performance for 
each inner most loop iteration, this would imply that mat-mat CSC/CSR are able to load keep more data in cache than CSR/CSC mat-vec.  The main advantage of
the sparse mat-mat operations is less drop off in performance for increased N. AOS implementation never exceeded 0.8 GFlop/s and COO never
exceeded 0.6 GFLops/s.  This suggests that the row/col compression significantly improves performance.

4) How does the performance of sparse matrix by dense matrix product (in
GFLOP/s) compare to the results you got dense matrix-matrix product in
previous assignments?  Explain, and quantify if you can, using the
roofline model.

For a RHS of 512 from N=64 to N=1024: CSC mat-mat achieved a max Gflops/s of 10.3 and min of 8.7 GFlops.  This is very close to the the max roofline performace of 
~13.5 GFlops.  CSR was on the order of 8-7.8 GFlops/s, AOS was steady around 1.2 GFlops/s, and COO was steady around 3 GFlops/s.  The best performing 
mat-mat multiplication algorithms previously looked at were mat-mat with ikj loop ordering achieved a max of around 5 GFlops/s, and mult_2 achieved a max around
5 GFlops/s.  To achieve ~10 GFlops/s assuming (2 flops/16 bytes) implies around 0.13 flops/byte. At a rate of 10 GFlops/s, this corresponds approximately
to the L1 cache cutoff of the roofline plot. This would imply that most of the matrix-matrix data was able to be loaded in L1 cache for 
N=64 case (10 GFlops/s) and 8 GFlops/s (min at N=512) corresponds to the L2 cache cutoff for the same flops/byte. CSC seems to be the fastest implementation of the
sparse compressed matrices. Part of this superior performance could be attributed to: CSC loops through each column element in a row, which should
be faster than looping through each row element of a column (CSR) because matrix elements are loaded in by row, not column.

5) What was the most important thing learned from this assignment? What do you still not understand?

The most important thing I learned from this assignment was the understanding the use of and superior operation of compressed sparse matrices. One thing I still do not 
fully understand is quantifing the number of flops per operation in code. 
