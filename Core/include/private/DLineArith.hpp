/*
 * Copyright (c) 2011, Matthieu FAESSEL and ARMINES
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef _D_LINE_ARITH_HPP
#define _D_LINE_ARITH_HPP


#include "DImage.hpp"

#if defined SMIL_USE_SSE_INT && defined __SSE__
#include <emmintrin.h>
#endif // SMIL_USE_SSE_INT


/**
 * \ingroup Core
 * \defgroup Arith
 * @{
 */


template <class T>
struct fillLine : public unaryLineFunctionBase<T>
{
    inline void _exec(T *lInOut, int size, T value)
    {
	for (int i=0;i<size;i++)
	  lInOut[i] = value;
    }
};

template <class T>
struct invLine : public unaryLineFunctionBase<T>
{
    inline void _exec(T *lineIn, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	  lOut[i] = ~lineIn[i];
    }
};

template <class T>
struct addLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T* lIn1, T* lIn2, int size, T* lOut)
    {
	for(int i=0;i<size;i++)
	    lOut[i] = lIn1[i] > (T)(numeric_limits<T>::max()- lIn2[i]) ? numeric_limits<T>::max() : lIn1[i] + lIn2[i];
    }
    inline void _exec_aligned(T *lIn1, T *lIn2, int size, T *lOut) { _exec(lIn1, lIn2, size, lOut); }
};

#if defined SMIL_USE_SSE_INT && defined __SSE__

template <>
inline void addLine<UINT8>::_exec_aligned(UINT8 *lIn1, UINT8 *lIn2, int size, UINT8 *lOut)
{
     __m128i r0,r1;
    __m128i *l1 = (__m128i*) lIn1;
    __m128i *l2 = (__m128i*) lIn2;
    __m128i *l3 = (__m128i*) lOut;
    for(int i=0 ; i<size ; i+=16, l1++, l2++, l3++)
    {
	r0 = _mm_load_si128(l1);
	r1 = _mm_load_si128(l2);
	r1 = _mm_adds_epu8(r0, r1);
	_mm_store_si128(l3, r1);
    }
}

#endif // SMIL_USE_SSE_INT


template <class T>
struct addNoSatLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	  lOut[i] = lIn1[i] + lIn2[i];
    }
    inline void _exec_aligned(T *lIn1, T *lIn2, int size, T *lOut) { _exec(lIn1, lIn2, size, lOut); }
};

#if defined SMIL_USE_SSE_INT && defined __SSE__

template <>
inline void addNoSatLine<UINT8>::_exec_aligned(UINT8 *lIn1, UINT8 *lIn2, int size, UINT8 *lOut)
{
     __m128i r0,r1;
    __m128i *l1 = (__m128i*) lIn1;
    __m128i *l2 = (__m128i*) lIn2;
    __m128i *l3 = (__m128i*) lOut;
    for(int i=0 ; i<size ; i+=16, l1++, l2++, l3++)
    {
	r0 = _mm_load_si128(l1);
	r1 = _mm_load_si128(l2);
	r1 = _mm_add_epi8(r0, r1);
	_mm_store_si128(l3, r1);
    }
}

#endif // SMIL_USE_SSE_INT

template <class T>
struct subLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	  lOut[i] = lIn1[i] < (T)(numeric_limits<T>::max() + lIn2[i]) ? numeric_limits<T>::min() : lIn1[i] - lIn2[i];
    }
    inline void _exec_aligned(T *lIn1, T *lIn2, int size, T *lOut) { _exec(lIn1, lIn2, size, lOut); }
};

#if defined SMIL_USE_SSE_INT && defined __SSE__

template <>
inline void subLine<UINT8>::_exec_aligned(UINT8 *lIn1, UINT8 *lIn2, int size, UINT8 *lOut)
{
     __m128i r0,r1;
    __m128i *l1 = (__m128i*) lIn1;
    __m128i *l2 = (__m128i*) lIn2;
    __m128i *l3 = (__m128i*) lOut;
    for(int i=0 ; i<size ; i+=16, l1++, l2++, l3++)
    {
	r0 = _mm_load_si128(l1);
	r1 = _mm_load_si128(l2);
	r1 = _mm_subs_epu8(r0, r1);
	_mm_store_si128(l3, r1);
    }
}

#endif // SMIL_USE_SSE_INT

template <class T>
struct subNoSatLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	  lOut[i] = lIn1[i] - lIn2[i];
    }
    inline void _exec_aligned(T *lIn1, T *lIn2, int size, T *lOut) { _exec(lIn1, lIn2, size, lOut); }
};

#if defined SMIL_USE_SSE_INT && defined __SSE__

template <>
inline void subNoSatLine<UINT8>::_exec_aligned(UINT8 *lIn1, UINT8 *lIn2, int size, UINT8 *lOut)
{
     __m128i r0,r1;
    __m128i *l1 = (__m128i*) lIn1;
    __m128i *l2 = (__m128i*) lIn2;
    __m128i *l3 = (__m128i*) lOut;
    for(int i=0 ; i<size ; i+=16, l1++, l2++, l3++)
    {
	r0 = _mm_load_si128(l1);
	r1 = _mm_load_si128(l2);
	r1 = _mm_sub_epi8(r0, r1);
	_mm_store_si128(l3, r1);
    }
}

#endif // SMIL_USE_SSE_INT

template <class T>
struct supLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	  lOut[i] = lIn1[i] > lIn2[i] ? lIn1[i] : lIn2[i];
    }
    inline void _exec_aligned(T *lIn1, T *lIn2, int size, T *lOut) { _exec(lIn1, lIn2, size, lOut); }
};

#if defined SMIL_USE_SSE_INT && defined __SSE__

template <>
inline void supLine<UINT8>::_exec_aligned(UINT8 *lIn1, UINT8 *lIn2, int size, UINT8 *lOut)
{
     __m128i r0,r1;
    __m128i *l1 = (__m128i*) lIn1;
    __m128i *l2 = (__m128i*) lIn2;
    __m128i *l3 = (__m128i*) lOut;
    for(int i=0 ; i<size ; i+=16, l1++, l2++, l3++)
    {
	r0 = _mm_load_si128(l1);
	r1 = _mm_load_si128(l2);
	r1 = _mm_max_epu8(r0, r1);
	_mm_store_si128(l3, r1);
    }
}

#endif // SMIL_USE_SSE_INT

template <class T>
struct infLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	  lOut[i] = lIn1[i] < lIn2[i] ? lIn1[i] : lIn2[i];
    }
    inline void _exec_aligned(T *lIn1, T *lIn2, int size, T *lOut) { _exec(lIn1, lIn2, size, lOut); }
};

#if defined SMIL_USE_SSE_INT && defined __SSE__

template <>
inline void infLine<UINT8>::_exec_aligned(UINT8 *lIn1, UINT8 *lIn2, int size, UINT8 *lOut)
{
     __m128i r0,r1;
    __m128i *l1 = (__m128i*) lIn1;
    __m128i *l2 = (__m128i*) lIn2;
    __m128i *l3 = (__m128i*) lOut;
    for(int i=0 ; i<size ; i+=16, l1++, l2++, l3++)
    {
	r0 = _mm_load_si128(l1);
	r1 = _mm_load_si128(l2);
	r1 = _mm_min_epu8(r0, r1);
	_mm_store_si128(l3, r1);
    }
}

#endif // SMIL_USE_SSE_INT


template <class T>
struct grtLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	  lOut[i] = lIn1[i] > lIn2[i] ? numeric_limits<T>::max() : 0;
    }
};

template <class T>
struct grtOrEquLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	  lOut[i] = lIn1[i] >= lIn2[i] ? numeric_limits<T>::max() : 0;
    }
};

template <class T>
struct lowLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	  lOut[i] = lIn1[i] < lIn2[i] ? numeric_limits<T>::max() : 0;
    }
};

template <class T>
struct lowOrEquLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	  lOut[i] = lIn1[i] <= lIn2[i] ? numeric_limits<T>::max() : 0;
    }
};

template <class T>
struct equLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	  lOut[i] = lIn1[i] == lIn2[i] ? numeric_limits<T>::max() : 0;
    }
};

template <class T>
struct difLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	  lOut[i] = lIn1[i] != lIn2[i] ? numeric_limits<T>::max() : 0;
    }
};


template <class T>
struct mulLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	    lOut[i] = double(lIn1[i]) * double(lIn2[i]) > double(numeric_limits<T>::max()) ? numeric_limits<T>::max() : lIn1[i] * lIn2[i];
    }
    inline void _exec_aligned(T *lIn1, T *lIn2, int size, T *lOut) { _exec(lIn1, lIn2, size, lOut); }
};

#if defined SMIL_USE_SSE_INT && defined __SSE__

template <>
inline void mulLine<UINT8>::_exec_aligned(UINT8 *lIn1, UINT8 *lIn2, int size, UINT8 *lOut)
{
     __m128i r0,r1;
    __m128i *l1 = (__m128i*) lIn1;
    __m128i *l2 = (__m128i*) lIn2;
    __m128i *l3 = (__m128i*) lOut;
    for(int i=0 ; i<size ; i+=16, l1++, l2++, l3++)
    {
	r0 = _mm_load_si128(l1);
	r1 = _mm_load_si128(l2);
	r1 = _mm_mullo_epi16(r0, r1);
	_mm_store_si128(l3, r1);
    }
}

#endif // SMIL_USE_SSE_INT

template <class T>
struct mulNoSatLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	    lOut[i] = (T)(lIn1[i] * lIn2[i]);
    }
};

template <class T>
struct divLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	{
	    lOut[i] = lIn2[i]==0 ? numeric_limits<T>::max() : lIn1[i] / lIn2[i];
	}
    }
};

template <class T>
struct logicAndLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	    lOut[i] = (T)(lIn1[i] && lIn2[i]);
    }
};

template <class T>
struct logicOrLine : public binaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	    lOut[i] = (T)(lIn1[i] || lIn2[i]);
    }
};


template <class T>
struct testLine : public tertiaryLineFunctionBase<T>
{
    inline void _exec(T *lIn1, T *lIn2, T *lIn3, int size, T *lOut)
    {
	for (int i=0;i<size;i++)
	{
	    lOut[i] = lIn1[i] ? lIn2[i] : lIn3[i];
	}
    }
};


/** @}*/

#endif // _D_LINE_ARITH_HPP
