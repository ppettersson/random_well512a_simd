A SIMD optimized version of the WELL512a random algorithm.


=== OVERVIEW ===
WELL512a is a pseudorandom number generator algorithm developed by
F. Panneton, P. L'Ecuyer and M. Matsumoto.

This is a fast implementation of that algorithm in C++ using SSE2 intrinsics
for the x86 / x64 platform.

For a detailed description of how the algorithm works, please see the
following pages:
1) http://en.wikipedia.org/wiki/Well_equidistributed_long-period_linear
2) http://www.iro.umontreal.ca/~panneton/WELLRNG.html


=== PERFORMANCE ===
This implementation is generally about 2-3 times faster than the C
implementation by the original authors.

Some example numbers from the included test program run on an Intel Core
i7-3770K 3.5GHz:

                VS2012     gcc 4.6.3
C++ SSE2        0.078      0.07
Original C      0.223      0.19
-------------------------------------
                x2.86      x2.71


=== REQUIREMENTS ===
An SSE2 capable CPU (i.e. AMD Athlon 64, Intel Pentium 4 or newer.

It's been tested with Visual Studio 2012 and gcc 4.6.3 but should compile
and run on other compilers as well.


=== LICENSE ===
All the source code, except WELL512a.cpp|h that contains the original
implementation, are released under the Simplified BSD license which allows
you to use this code in any open or closed source project.


=== ACKNOWLEDGEMENTS ===
Thanks to Marcus Geelnard for code review and many suggestions for
improvements.
