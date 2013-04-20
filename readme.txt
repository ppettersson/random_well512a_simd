A SIMD optimized version of the WELL512a random algorithm.

For a detailed description of how the algorithm works, please see the following pages:
1) http://en.wikipedia.org/wiki/Well_equidistributed_long-period_linear
2) http://www.iro.umontreal.ca/~panneton/WELLRNG.html

Benchmarking shows that the SSE2 implementation is generally about 2-3 times faster than
the C implementation by the original authors.

It requires a SSE2 capable CPU (i.e. Intel Pentium 4 or newer, AMD Athlon 64 or newer).

All the source code except WELL512a.cpp|h that contains the original implementation
are released under the BSD license which allows you to include this in any open
or closed source project.
