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

#include "RandomWELL512a_SSE2.h"
#include <stdlib.h>		// Needed for rand().
#include <memory.h>		// Needed for memcpy().

RandomWELL512a_SSE2::RandomWELL512a_SSE2(int seed)
	: index(0)
	, resultIndex(4)
{
	srand(seed);
	for (int i = 0; i < 16; ++i)
		xmm_state[i] = _mm_set_epi32(rand(), rand(), rand(), rand());
}

RandomWELL512a_SSE2::RandomWELL512a_SSE2(unsigned *seed)
	: index(0)
	, resultIndex(4)
{
	for (int i = 0; i < 16; ++i)
		xmm_state[i] = _mm_set_epi32(seed[i + 48], seed[i + 32], seed[i + 16], seed[i]);
}

void RandomWELL512a_SSE2::GetUnsigned4(unsigned *result4)
{
	// Calculations.
	#define MUTATE_LEFT(value, shift)			_mm_xor_si128(value, _mm_slli_epi32(value, shift))
	#define MUTATE_RIGHT(value, shift)			_mm_xor_si128(value, _mm_srli_epi32(value, shift))
	#define MUTATE_LEFT_MIX(value, shift, mix)	_mm_xor_si128(value, _mm_and_si128(_mm_slli_epi32(value, shift), mix))

	unsigned index_15		= (index + 15) & 15;
	__m128i state_index		= xmm_state[index];
	__m128i state_index_9	= xmm_state[(index +  9) & 15];
	__m128i state_index_13	= xmm_state[(index + 13) & 15];
	__m128i state_index_15	= xmm_state[index_15];
	const __m128i kMix		= _mm_set1_epi32(0xda442d24);

	__m128i z1 = _mm_xor_si128(MUTATE_LEFT(state_index, 16), MUTATE_LEFT(state_index_13, 15));
	__m128i z2 = MUTATE_RIGHT(state_index_9, 11);
	__m128i result0 = _mm_xor_si128(z1, z2);
	xmm_state[index] = result0;

	__m128i result1 = MUTATE_LEFT(state_index_15, 2);
	result1 = _mm_xor_si128(result1, MUTATE_LEFT(z1, 18));
	result1 = _mm_xor_si128(result1, _mm_slli_epi32(z2, 28));
	result1 = _mm_xor_si128(result1, MUTATE_LEFT_MIX(result0, 5, kMix));
	index = index_15;
	xmm_state[index] = result1;

	_mm_storeu_si128((__m128i *)result4, result1);

	#undef MUTATE_LEFT
	#undef MUTATE_RIGHT
	#undef MUTATE_LEFT_MIX
}

void RandomWELL512a_SSE2::GetDouble4(double *result4)
{
	unsigned unsignedResult[4];
	GetUnsigned4(unsignedResult);

	const double kToFloat = 2.32830643653869628906e-10;
	for (unsigned loop = 0; loop < 4; ++loop)
		result4[loop] = unsignedResult[loop] * kToFloat;
}
