


1. What level of SIMD/vector support does the CPU your computer provide?

Intel i5 (2012):
CPUID_ECX_SSE3
CPUID_ECX_SSSE3
CPUID_ECX_SSE41
CPUID_ECX_SSE42
CPUID_ECX_AVX

2. What is the maximum operand size that your computer will support?

The maximum supported operand size is 256 bits (AVX).

3. How many double-precision floating point numbers can fit into one operand given the maximum operand size that your CPU will support?

Assuming the double-precision float takes up the standard 64 bits, then a maximum of 4 double-precision floats can fit into one 256 bit operand. 

4. What is the clock speed of your CPU?  (You may need to look this up via "About this Mac" or "lscpu").

Intel i5-3210 (2012): 2.5 GHz clock speed - max 2.9 GHz dual-core, 3.1 GHz single-core

5. Based on the output from bandwidth.exe on your computer, what do you expect L1 cache and L2 cache sizes to be?  What are the corresponding bandwidths?   How do the cache sizes compare to what "about this mac" (or equivalent) tells you about your CPU?  (There is no "right" answer for this question -- but I do want you to do the experiment.)

I do not see a consistent discontinuous drop in bandwidth from 128 bytes to 8388608 bytes, which is well past L1 cache size. Upon researching my procressor (i5-3210), the L1 cache is 32 KB, L2 cache is 256 KB, and L3 is 3MB.  Intel employs a "Smart Cache" architecture that allows all cores to dynamically share access to the last level cache,
I wonder if this smooths out the transition between caches so the bandwidth doesn't singificantly change? Because of these confusing results, I'll pursue the Docker path.

6. Based on the output from Docker bandwidth program on your computer, what do you expect L1 cache and L2 cache sizes to be?  What are the corresponding bandwidths?   How do the cache sizes compare to what "about this mac" (or equivalent) tells you about your CPU?  (There is no "right" answer for this question -- but I do want you to do the experiment.)

The max peak bandwidth is around 80 GBits/s.  The first sharp drop off in bandwidth performance is around 33 KB, which corresponds to the rated L1 cahce of (32 KB).  The second, less pronounced drop off is around 250 KB, which corresponds to the L2 cache (256 KB).  The third drop off occurs around 2MB, which corresponds to the L3 cache.  
I didn't see such stark bandwidth changes in the bandwidth.exe program, so it is hard to compare. The maximum bandwidth I recorded using bandwidth.exe was 83 Gbits/s, which is around the max of ~80 Gbits/s measured by the dock bandwidth program.  

7. What is the (potential) maximum compute performance of your computer?  (The horizontal line.)  What are the L1, L2, and RAM bandwidths?  How do those bandwidths correspond to  what was measured above with the bandwidth program?

I did not pursue the homegrown approach past Q5.

8. Based on the clock speed of your CPU and its maximum Glop rate, what is the (potential) maximum number of *double precision* floating point operations that can be done per clock cycle?  (Hint: Glops / sec :math:`\div` GHz = flops / cycle.)  There are several hardware capabilities that can contribute to supporting more than one operation per cycle: fused multiply add (FMA) and AVX registers.  Assuming FMA contributes a factor of two, SSE contributes a factor of two,  AVX/AVX2 contribute a factor of four, and AVX contributes a factor of eight of eight, what is the expected maximum number of floating point operations your CPU could perform per cycle, based on the capabilities your CPU advertises via cpuinfo (equiv. lscpu)?  Would your answer change for single precision (would any of the previous assumptions change)?  

While I did not pursue the homegrown apprach past Q5, I do know that a single precision float typically takes 32 bits, which is half the bits of a double precision number. This means that for each register size, 2x as many single precision numbers can be included versus double precision numbers.

9. What is the maximum compute performance of your computer?  (The horizontal line.)  What are the L1, L2, and DRAM bandwidths?  How do those bandwidths correspond to what was measured above?

The maximum performance I measured is 13.3 Gflops/sec.  L1 cache bandwidth was measured to be 87.6 GB/s, L2 cache bandwidth was measured to be 50.1 GB/s, L3 cache bandwidth was measured to be 36.3 GB/s,
and DRAM bandwidth was measured to be 14.3 GB/s.  Compared to the results from question 6, the L1 cache bandwidths are about the same magnitude (~81 GB/s).  From question 6, the max L2 bandwidth was measured
to be ~37 GB/s, which is significantly less than 50.1 GB/s measured in Q9.  From question 6, the L3 cache was measured to be ~25 GB/s, this is also significantly less than the 36.3 GB/s measured in Q9.  Finally,
the DRAM bandwidth was measured to be ~13 GB/s, which is close the measured peak of 14.3 GB/s in Q9. 

10. Referring to the figures about how data are stored in memory, what is it about the best performing pair of loops that is so advantageous?

The clear best performing loop ordering is ikj, the second best performing is kij.  The best ordering involves having the j-loop as the inner-most loop.
The worst performing loops kji and jki both have the i-loop as the inner most loop. I'll analyze the performance of the ikj odering first. By accessing i first, the ith row of A is 
loaded into cache since the matrices are row ordered. Next, by accessing k we're loading in the rows of B next and looping through the already loaded in columns of A(i,...). Finally, by looping 
through j, we're now looping through the columns of B(k,...), which have already been loaded into cache by first loading in the row B(k,...).  By loading looping through j last, the algorithm
ensures that the columns of B(k,...) will already be loaded into cache, so the the next element B(k,j+1) will be easier to access than if the inner loop had B looping through rows in the inner-most loop.  

11. What will the data access pattern be when we are executing ``mult_trans`` in i,j,k order?  What data are accessed in each if the matrices at step (i,j,k) and what data are accessed at step (i, j, k+1)? Are these accesses advantageous in any way?

In the mult_trans functions, the transpose of B is performed before the multiplication loops, creating a new matrix B_t.  This transforms the rows of B to the collumns of B_t and the collumns of B to the rows
of B_t. Normal matrix multiplication multiplies a row of A into a column of B. The CPU loads in a row of a matrix into cache, making iterating by collumns of a row cheap and iterating by collumns of a row expensive
since multiple rows aren't loaded into cache. When iterating in i,j,k order, matrix A is accessed as A(i,k), by loading in a row of A then iterating through the collumns along k of row A(i,..). This is computationally cheap
as the whole row of A(i,..) is brough into cache.  With B_t(k,j) as the transpose of B, when B_t(k,...) is loaded in, it really is a collumn of B and k+1 loads in the next column of B (row of B_t).  Then in the outer
loop of j+1, the columns of B_t (rows of B) are looped through.  By using the transpose of B (B_t) in the multiplication, the correct multiplication form of rows of A times columns of B is preserved while also allowing for
faster memory access by loading in the columns of B (rows of B_t) first then iterating. 

12. Referring again to how data are stored in memory, explain why hoisting  ``C(i,j)`` out of the inner loop is so beneficial in mult_trans with the "ijk" loop ordering.

Because the vaue stored in C(i,j) is not needed in the evaluation of A(i,k)*B(k,j), having to load C(i,...) while looping through k is unnecessarily expensive.  By accessing a single accumulator
variable t, we save memory access time by repeatedly accessing the same location t, instead of location C(i,j) every k iteration.

13. What optimization is applied in going from ``mult_2`` to ``mult_3``?

mult_2 employs hoisting and tiling.  mult_3 builds on mult_2 by emplying hoisting, tiling, and blocking.  Hoisting prevents the cpu
from having to access a new memory location of C(i,j) within the innermost loop.  Tiling builds off of hoisting by accessing nearby memory locations all at once
requiring fewer calls to C(i,j) memory locations.  Blocking seperates the input matrices into smaller 'blocks' that can fit into cache all at once, preventing the cpu from having to access
different caches or memory to find/access data.

14. How does your maximum achieved performance for ``mult`` (any version) compare to what bandwidth and roofline predicted?

The best performing mmult function was mult_ikj with a max of ~6 GFlop/s.  This is a little under half of the maximum performance predicted by
the roofline of ~13 Gflops/s.



