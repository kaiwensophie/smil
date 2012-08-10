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
 *     * Neither the name of Matthieu FAESSEL, or ARMINES nor the
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


#ifndef _D_MORPHO_EXTREMA_HPP
#define _D_MORPHO_EXTREMA_HPP

#include "DMorphoGeodesic.hpp"
#include "DImageArith.hpp"

/**
 * \addtogroup Morpho
 * \{
 */


// Extrema

/**
 * h-Minima
 */
template <class T>
RES_T hMinima(const Image<T> &imIn, const T &height, Image<T> &imOut, const StrElt &se=DEFAULT_SE)
{
    if (&imIn==&imOut)
    {
	Image<T> tmpIm = imIn;
	return hMinima(tmpIm, height, imOut, se);
    }
    
    StrElt tmpSe(se);
    tmpSe.size = 1;
    
    RES_T res;
    
    res = add(imIn, T(height), imOut);
    if (res!=RES_OK)
      return res;
    
    res = dualBuild(imOut, imIn, imOut, tmpSe);
    if (res!=RES_OK)
      return res;
    
    low(imIn, imOut, imOut);
    
    return res;
}

/**
 * h-Maxima
 */
template <class T>
RES_T hMaxima(const Image<T> &imIn, const T &height, Image<T> &imOut, const StrElt &se=DEFAULT_SE)
{
    if (&imIn==&imOut)
    {
	Image<T> tmpIm = imIn;
	return hMaxima(tmpIm, height, imOut, se);
    }
    
    StrElt tmpSe(se);
    tmpSe.size = 1;
    
    RES_T res;
    
    res = sub(imIn, T(height), imOut);
    if (res!=RES_OK)
      return res;
    
    res = build(imOut, imIn, imOut, tmpSe);
    if (res!=RES_OK)
      return res;
    
    grt(imIn, imOut, imOut);
    
    return res;
}

/**
 * Minima
 */
template <class T>
RES_T minima(const Image<T> &imIn, Image<T> &imOut, const StrElt &se=DEFAULT_SE)
{
    return hMinima(imIn, T(1), imOut, se);
}

/**
 * Maxima
 */
template <class T>
RES_T maxima(const Image<T> &imIn, Image<T> &imOut, const StrElt &se=DEFAULT_SE)
{
    return hMaxima(imIn, T(1), imOut, se);
}

/** \} */

#endif // _D_MORPHO_EXTREMA_HPP

