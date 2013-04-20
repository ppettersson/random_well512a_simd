// Copyright (c) 2013, Peter Pettersson
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are those
// of the authors and should not be interpreted as representing official policies,
// either expressed or implied, of the FreeBSD Project.

#include "Timer.h"
#include "RandomWELL512a.h"
#include "RandomWELL512a_SSE2.h"
#include "WELL512a.h"
#include <string.h>     // Needed for memcmp().

void Benchmark()
{
	const unsigned kSeed = 123;
	const unsigned kNumLoops = 5;
	const unsigned kNumIterations = 40000000;
	double *result0 = new double [kNumIterations];
	double *result1 = new double [kNumIterations];
	double *result2 = new double [kNumIterations];
	double *result3 = new double [kNumIterations];

	srand(kSeed);
	unsigned seed[16];
	for (unsigned i = 0; i < 16; ++i)
		seed[i] = rand();

	for (unsigned loop = 0; loop < kNumLoops; ++loop)
	{
		// RAND implementation.
		srand(kSeed);
		Timer timer0;
		for (unsigned i = 0; i < kNumIterations; ++i)
			result0[0] = rand() / (double)RAND_MAX;
		timer0.Report("Rand():       ");

		// WELL512 C++ implementation.
		RandomWELL512a random(kSeed);
		Timer timer1;
		for (unsigned i = 0; i < kNumIterations; ++i)
			result1[i] = random.GetDouble();
		timer1.Report("WELL512 C++:  ");

		// WELL512 SSE2 implementation.
		RandomWELL512a_SSE2 randomSSE2(kSeed);
		Timer timer2;
		for (unsigned i = 0; i < kNumIterations; i += 4)
			randomSSE2.GetDouble4(result2 + i);
		timer2.Report("WELL512 SSE2: ");

		// WELL512 C implementation.
		InitWELLRNG512a(seed);
		Timer timer3;
		for (unsigned i = 0; i < kNumIterations; ++i)
			result3[i] = WELLRNG512a();
		timer3.Report("WELL512 C:    ");

		std::cout << "---" << std::endl;
	}

	delete [] result0;
	delete [] result1;
	delete [] result2;
	delete [] result3;
}

// Verify that the SIMD implementation returns the same values as the original
// algorithm would.
void Test()
{
	const int kSeed = 123;
	const unsigned kNumIterations = 4 * 1024;

	double *result0 = new double [kNumIterations];
	double *result1 = new double [kNumIterations];

	srand(kSeed);
	unsigned seed[4 * 16];
	for (unsigned i = 0; i < 4 * 16; ++i)
		seed[i] = rand();
	RandomWELL512a randomWell0(seed + 0 * 16);
	RandomWELL512a randomWell1(seed + 1 * 16);
	RandomWELL512a randomWell2(seed + 2 * 16);
	RandomWELL512a randomWell3(seed + 3 * 16);

	RandomWELL512a_SSE2 randomWellSSE2(seed);

	for (unsigned i = 0; i < kNumIterations; i += 4)
	{
		result0[i + 0] = randomWell0.GetDouble();
		result0[i + 1] = randomWell1.GetDouble();
		result0[i + 2] = randomWell2.GetDouble();
		result0[i + 3] = randomWell3.GetDouble();

		result1[i + 0] = randomWellSSE2.GetDouble();
		result1[i + 1] = randomWellSSE2.GetDouble();
		result1[i + 2] = randomWellSSE2.GetDouble();
		result1[i + 3] = randomWellSSE2.GetDouble();
	}

	if (memcmp(result0, result1, kNumIterations * sizeof(double)))
		std::cout << "ERROR: C++ vs SSE2: The results don't match!" << std::endl;
	else
		std::cout << "C++ vs SSE2: Results match" << std::endl;

	delete [] result0;
	delete [] result1;
}

int main(int argc, char **argv)
{
	Benchmark();
	Test();
	return 0;
}
