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


#ifndef _D_IMAGE_DRAW_HPP
#define _D_IMAGE_DRAW_HPP

#include "DLineArith.hpp"

/**
 * \ingroup Base
 * \defgroup Draw
 * @{
 */


/**
 * Draw a rectangle
 * 
 * 
 * \param imOut Output image.
 */
template <class T>
inline RES_T drawRectangle(Image<T> &imOut, UINT x0, UINT y0, UINT width, UINT height, T value=numeric_limits<T>::max(), bool fill=false)
{
    if (!imOut.isAllocated())
        return RES_ERR_BAD_ALLOCATION;

    UINT imW = imOut.getWidth();
    UINT imH = imOut.getHeight();
    
    if (x0>=imW || y0 >=imH)
      return RES_ERR;
    if (x0+width>=imW || y0+height>=imH)
      return RES_ERR;
    
    UINT x1 = x0 + width - 1;
    UINT y1 = y0 + height -1;
    x1 = x1<imW ? x1 : imW-1;
    y1 = y1<imH ? y1 : imH-1;
    
    x0 = x0>0 ? x0 : 0;
    y0 = y0>0 ? y0 : 0;
    
    typename Image<T>::sliceType lines = imOut.getLines();
    fillLine<T> fillFunc;
    
    if (fill)
    {
	for (UINT j=y0;j<=y1,j<imOut.getHeight();j++)
	  fillFunc(lines[j]+x1, width-1, value);
    }
    else
    {
	fillFunc(lines[y0]+x0, width, value);
	fillFunc(lines[y1]+x0, width, value);
	for (UINT j=y0+1;j<=y1;j++)
	{
	    lines[j][x0] = value;
	    lines[j][x1] = value;
	}
    }
    
    imOut.modified();
    
    return RES_OK;
}



/** @}*/

#endif // _D_IMAGE_DRAW_HPP
