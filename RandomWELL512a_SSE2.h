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

#ifndef RANDOM_WELL512A_SSE2_H
#define RANDOM_WELL512A_SSE2_H

#include <emmintrin.h>	// SSE2 instrinsics.

class RandomWELL512a_SSE2
{
public:
	RandomWELL512a_SSE2(int seed);
	RandomWELL512a_SSE2(unsigned *seed);

	unsigned GetUnsigned()
	{
		if (resultIndex < 4)
			return result[resultIndex++];

		GetUnsigned4(result);
		resultIndex = 1;
		return result[0];
	}

	double GetDouble()
	{
		const double kToFloat = 2.32830643653869628906e-10;
		return GetUnsigned() * kToFloat;
	}


	void GetUnsigned4(unsigned *result4);
	void GetDouble4(double *result4);

private:
	__m128i xmm_state[16];
	unsigned index;

	// Helper to allow us to return one number per call.
	unsigned result[4];
	unsigned resultIndex;
};

#endif // RANDOM_WELL512A_SSE2_H
