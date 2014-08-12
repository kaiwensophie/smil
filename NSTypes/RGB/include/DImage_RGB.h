/*
 * Copyright (c) 2011-2014, Matthieu FAESSEL and ARMINES
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


#ifndef _IMAGE_RGB_H
#define _IMAGE_RGB_H


#include "Core/include/private/DImage.hpp"
// #include "Core/include/DColor.h"
#include "Base/include/private/DImageArith.hpp"
#include "Core/include/private/DMultichannelImage.hpp"
#include "Base/include/private/DImageTransform.hpp"
#include "Base/include/private/DMeasures.hpp"

namespace smil
{
  
//     template <>
//     inline void Image<RGB>::init() 
//     { 
// 	className = "Image";
// 	
// 	slices = NULL;
// 	lines = NULL;
//     //     pixels = NULL;
// 
// 	dataTypeSize = sizeof(pixelType); 
// 	
// 	allocatedSize = 0;
// 	
// 	viewer = NULL;
// 	name = "";
// 	
// 	updatesEnabled = true;
// 	
// 	parentClass::init();
//     }

   
//     template <>
//     inline std::map<RGB, UINT> histogram(const Image<RGB> &imIn)
//     {
// 	map<T, UINT> h;
// 	for (T i=ImDtTypes<T>::min();;i++)
// 	{
// 	    h.insert(pair<T,UINT>(i, 0));
// 	    if (i==ImDtTypes<T>::max())
// 	      break;
// 	}
// 
// 	typename Image<T>::lineType pixels = imIn.getPixels();
// 	for (size_t i=0;i<imIn.getPixelCount();i++)
// 	    h[pixels[i]]++;
// 	
// 	return h;
//     }

//     template <>
//     inline void Image<RGB>::printSelf(ostream &os, bool displayPixVals, bool hexaGrid, string indent) const
//     {
//     }
    
    
    
#ifdef USE_QWT
// #include "Gui/Qt/DQtImageViewer.hpp"
#endif // USE_QWT
    
    template <class T, UINT N>
    inline RES_T mul(const Image< MultichannelType<T,N> > &imIn, const double &dValue, Image< MultichannelType<T,N> > &imOut)
    {
      return RES_ERR_NOT_IMPLEMENTED;
    }

    template <class T, UINT N, class T2>
    inline RES_T stretchHist(const Image< MultichannelType<T,N> > &imIn, Image<T2> &imOut, T2 outMinVal, T2 outMaxVal)
    {
      return RES_ERR_NOT_IMPLEMENTED;
    }
    
    template <class T, UINT N, class T1>
    inline RES_T stretchHist(const Image<T1> &imIn, Image< MultichannelType<T,N> > &imOut, MultichannelType<T,N> outMinVal, MultichannelType<T,N> outMaxVal)
    {
      return RES_ERR_NOT_IMPLEMENTED;
    }
    
    
} // namespace smil

#endif // _IMAGE_RGB_H
