
PS3 Questions
=============

Add your answers to this file in plain text after each question.  Leave a blank line between the text of the question and the text of your answer.

argv
----

1. What does `argv[0]` always contain?

The path to executable file being called. Example: 'argv[0] = ./main.exe'

2. Which entry of `argv` holds the first argument passed to the program?

'argv[1]' holds the first command line argument passed to the program.

3. Which entry of `argv` holds the second argument passed to the program?

'argv[2]' holds the second argument passed to the program.

4. How would you print just the last argument passed to a program?

Assuming the variables 'int argc' and 'char* argv' are initialized when calling main, the last
element can be printed by writing:
    std::cout << "Last argument: " << argv[argc-1];

float vs double
----------------

5.  What is the difference (ratio) in execution times 
between single and double precision for    *construction with no optimization*? Explain.

-Averaged results over 3 runs per loop size.
Running 10^5 constructions, float-time/double-time avg: 0.925
Running 10^6 constructions, float-time/double-time avg: 0.988
Running 10^7 constructions, float-time/double-time avg: 0.908
Running 10^8 constructions, float-time/double-time avg: 0.970
Total construction float-time/double-time avg: 0.950
This is surprising since double (16 bytes) is twice as large as float (8 bytes). Initially I suspected that initializing 
doubles would take significatly longer than initializing floats since they differ in size by a factor of two.  However, floats 
suprisingly take only slightly less time to initialize than doubles. This suggests that modern cpus do not seem to have serious issue contructing doubles vs. floats. 
Exectuting the instructions for initializing doubles and floats must be very similar in the amout of operations they require, so
fast exectution of initialization relies less on data type and more on how the machine fetches instructions and writes to memory.

6.  What is the difference (ratio) in execution times
between single and double precision for    *multiplication with no optimization*? Explain.

-Averaged results over 3 runs per loop size.
Running 10^5 multiplications, float-time/double-time avg: 1.000
Running 10^6 multiplications, float-time/double-time avg: 0.861
Running 10^7 multiplications, float-time/double-time avg: 0.923
Running 10^8 multiplications, float-time/double-time avg: 1.145
Total multiplication float-time/double-time avg: 0.992
Again, with floats being smaller and less precise than doubles, I would expect floats to take less time to execute than doubles.  As it happens,
floats seems to take about the same time to fetch from memory and to perform muliplication on as do doubles. This implies that
it takes about the same number of cycles to fetch the data from memory, and perform the multiplication.  What I'm still not sure about
is how the cpu is able to handle the different levels of precision between the two numbers without taking noticibly more time for doubles.
I suspect that I'm not performing enough multiplications to see a large difference between the two. My Intel i5 runs at 2.5 GHz,
so I wonder if anything below 10^9 flops is relatively easy for the computer to handle regardless of precision. Out of curiosity,
I ran an optimized float_vs_double case for 10^9 operations and the float-time/double-time was 0.40 suggesting I didn't test high enough to see the effect.
There is a large varaince in the average results though. This suggests that there is more going on with the cpu than simply fetching instructions and data points.

7.  What is the difference (ratio) in execution times 
between single and double precision for    *construction with optimization*? Explain.
-Averaged results over 3 runs per loop size.
Running 10^5 constructions, float-time/double-time avg: 0.667
Running 10^6 constructions, float-time/double-time avg: 0.617
Running 10^7 constructions, float-time/double-time avg: 0.553
Running 10^8 constructions, float-time/double-time avg: 0.788
Total construction float-time/double-time avg: 0.653
This is the result I expected to see with the un-optimized code - doubles taking almost twice as long as floats to initialize.
I suspect that without optomization, the compilier doesn't care about the size difference between the variables and simply takes the same 
amount of time to initialize any variable of similar type since it probably assumes they're all the same size as the largest type.
With optimization, the compilier detects the large size difference bewteen float and double and is able to tell the cpu it needs less instructions
and space to initialize a float than a double due to the difference in size.  The cpu doesn't know to care about speed unless the compilier tells it.

8.  What is the difference (ratio) in execution times 
between single and double precision for    *multiplication with optimization*? Explain.

-Averaged results over 3 runs per loop size.
Running 10^5 multiplications, float-time/double-time avg: inf/nan
Running 10^6 multiplications, float-time/double-time avg: 1.5
Running 10^7 multiplications, float-time/double-time avg: 0.571
Running 10^8 multiplications, float-time/double-time avg: 1.467
Total multiplication float-time/double-time avg: 1.177
Now the opposite of what I expected occured, floats took on average longer to multiply than double.  There is a large
variance in the results, so I wonder again if the number of operations is below the clock speed of the cpu, the execution time 
can vary wildly regardless of the data type.  Then once the number of operations is on the same order of magnitude as the
clock speed, the relative performace bifurcates for different data types. I wonder if the large variation in exectution
times has something to do with pipeline stalls or something similar. 

9.  What is the difference (ratio) in execution times 
for double precision    multiplication with and without optimization? Explain. 

-Averaged results over 3 runs per loop size.
Running 10^5 multiplications, op-time/unop-time avg: 0 (excluded from average)
Running 10^6 multiplications, op-time/unop-time avg: 0.214
Running 10^7 multiplications, op-time/unop-time avg: 0.169
Running 10^8 multiplications, op-time/unop-time avg: 0.192
Total multiplication float-time/double-time avg: 0.192
Interestingly enough, the effect of the optimization stayed effectively constant over the different input sizes.  
This implies that the optimization has little to nothing to do with optimizing based upon the input data (which makes
sense since the compilier doesn't know the inputs before hand) and deals solely through optimizing the operations performed in the code.
What these optimizations are specifically is till opaque to me, but it has to do with how the compilier instructs the cpu to fetch and manipulae data.
From what little I understand, the main tradeoff with using optimization vs no optimization is compiling with no optimization
requires less time than compiling with optimization. 


Efficiency
----------

10.  If you double the problem size for matrix-matrix product, how many more operations will matrix-matrix product perform?

Matrix multiplication is at worst order N^3 operations (for square matrices). If you double the number of elements in each matrix from N to 2N,
you now have 3 matrices of 2N elements for 6N total elements.  This means that you have to do (2N)^3/N^3 = 8 times as many operations.

11.  If the time to perform a given operation is constant, when you double the problem size for matrix-matrix product, how much more time will be required to compute the answer?

Assuming constant time per each operation, the doubling of the problem size means 8x as many required operations (O(N^3)): so the problem will take ~8 times as long when doubled.  

12.  What ratio did you see when doubling the problem size when mmtime called `mult_0`?  (Hint, it may be larger than what pure operation count would predict.)  Explain.

I averaged the times for 128-4096 total elemets for mmtime.  The average increase in time for doubling the problem size is 13. This suggests that without code optimization,
there are multiple more steps taken by the compilier to perform the basic multiplication. These extra fetch data and instruction steps significatly slow down the exectution of the code.
Without boosting the code through improving locality, the approaching theoretical minimum of 8x per doubling of input data is impossible.  The cpu therefore spends extra time
accessing memory locations that could be sped up.  The performance below input size of 128 is difficult to measure with the current code since all runs take << 1 ms and aren't 
captured by the timer as written.

13.  What raio did you see when doubling the problem size when mmtime called `mult_3`?  Was this the same for `mult_0`?  Referring to the function in amath583.cpp, what optimizations are implemented and what kinds of performance benefits might they provide?

I averaged the times for 128-4096 total elemets for mmtime.  The average increase in time for doubling the problem size is 9. This suggests that with optimization,
the code is compilied with enough efficiency to achieve the same order of magnitude as the theoretical time increase minimum (8x). The mult_3 implementation makes
use of hoisting (improving temporal locality), tiling (improving spatial locality), and blocking (improving spatial locality).  Hoisting prevents the cpu
from having to access a new memory location of C(i,j) within the innermost loop.  Tiling builds off of hoisting by accessing nearby memory locations all at once
requiring fewer calls to C(i,j) memory locations.  Blocking seperates the input matrices into smaller 'blocks' that can fit into cache all at once, preventing the cpu from having to access
different caches or memory to find/access data.  

14. (Extra credit.)  Try also with `mult_1` and `mult_2`.

I averaged the times for 128-4096 total elemets for mmtime.  For mult_1, I achieved an averagve doubling the problem size time factor of ~12.  This suggests that only implementing
hoisting isn't very good for speeding up matrix multiplication compared to no optimization (13x).  For mult_2, I see a speed I achieved an averagve doubling the problem size time factor of ~10.
This also implies that hoisting and tiling combined aren't significant improvements but still better than no optimization.  Again, it's hard to make the comparison because the variance in the 
execution time required for the same number of loops varies strongly.  I do not feel comfortable calling my answers statisticaly significant. 

All-Pairs
---------

15. What do you observe about the different approaches to doing the similarity computation?  Which algorithm (optimizations) are most effective?  Does it pay to make a transpose of A vs a copy of A vs just passing in A itself.  What about passing in A twice vs passing it in once (mult_trans_3 vs mult_trans_4)?

Both mult_0/mult_trans_0 were too slow to perform above 128 images.  The same goes for mult_1/mult_trans_1, they were too slow to perform above 128 images. mult_1/mult_trans_1 was fast enough to analyze 256 images, but no more.  Both mult_3/mult_trans_3 and mult_trans_4 were fast enough to
perform from 128 images to 2048 images.  Both mult_trans_3 and mult_trans_4 seem to perform faster for 128 images at ~5.1 GFLOPS compared to ~4.3 GFLOPS for mult_3.  However, for 256 images and above, mult_3/mult_trans_3 and mult_trans_4 perform similarly between 2.5-3.5 GFLOPS.  It appears that
passing A twice versus copying A doesn't seem to improve the speed significantly. The biggest contribution to speed seems to be blocking the matrices. Hoisting and tiling seem to only offer small improvements (mult_1 and mult_2) - although without implementing blocking by itself, its hard
to determine its singular effect. Also, taking the tranpose of A does not seem to significantly improve the efficiency either. 

16. What is the best performance over all the algorithms that you observed for the case of 1024 images?  What would the execution time be for 10 times as many images?  For 60 times as many images?  (Hint: the answer is not cubic but still potentially a problem.)  What if we wanted to do, say 56 by 56 images instead of 28 by 28?

Both m_t_4(A) and m_t_3(A,A) seem to perform the best for 1024 images at ~2.87 GFLOPS (compared to ~2.80 for mult_3(A,B) and ~2.78 for m_t_3(A,C)).  I'm not seeing a clear superior algorithm bewteen m_t_4(A) and m_t_3(A,A) based off of these runs alone.
I got a factor of ~800 time increase for going from 128 to 1024 images for mult_trans_4, this is less than 10^3, but not by much. This implies as time increase ratio of around 170000 for 60 times as many images - this doesn't seem right, but I'm not sure how to test it. 
Considering 56 by 56 images instead of 28 by 28 results in an increase of 4x images.  This would imply around 50X longer based upon my my estimations - which seem suspect to me.


About PS3
---------


17. The most important thing I learned from this assignment was ...

Building a simple model of cpu/memory interaction to build intuition from. I do not come from a strong comp sci background, so this information is new and useful to me.

18. One thing I am still not clear on is ...

How the compilier works and what it does to optimize the compilation of code for different optimization levels.  I've read some on the topic, but it's still very opaque to me. 