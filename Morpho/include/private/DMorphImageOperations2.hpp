/*
 * Copyright (c) 2011-2016, Matthieu FAESSEL and ARMINES
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


#ifndef _MORPH_IMAGE_OPERATIONS2_HPP
#define _MORPH_IMAGE_OPERATIONS2_HPP

#include "Core/include/DCore.h"
#include "Morpho/include/DStructuringElement.h"
#include "Morpho/include/DMorphoInstance.h"

#ifdef USE_OPEN_MP
#include <omp.h>
#endif // USE_OPEN_MP

namespace smil
{
    /**
    * \ingroup Morpho 
    * \{
    */
  
    /**
     * Base morpho operator class.
     * 
     * \demo{custom_morpho_operator.py}
     */
  template <class T_in, class T_out=T_in>
    class Morph2ImageFunctionBase 
#ifndef SWIG
    : public imageFunctionBase<T_in>//binaryImageFunction< T_in, lineFunction_T >
#endif // SWIG
    {
    public:
        typedef Image<T_in> imageInType;
        typedef typename ImDtTypes<T_in>::lineType lineInType;
        typedef typename ImDtTypes<T_in>::sliceType sliceInType;
        typedef typename ImDtTypes<T_in>::volType volInType;
        
        typedef Image<T_out> imageOutType;
        typedef typename ImDtTypes<T_out>::lineType lineOutType;
        typedef typename ImDtTypes<T_out>::sliceType sliceOutType;
        typedef typename ImDtTypes<T_out>::volType volOutType;
        
        Morph2ImageFunctionBase(T_in _borderValue = ImDtTypes<T_in>::min(), T_out _initialValue = ImDtTypes<T_out>::min())
          : initialValue(_initialValue),
            borderValue(_borderValue)
        {
        }
        
        virtual ~Morph2ImageFunctionBase()
        {
        }
        
        
      RES_T operator()(const imageInType &imIn, const imageInType &imIn2, imageOutType &imOut, const StrElt &se=DEFAULT_SE) // BMI
        { 
	  return this->_exec(imIn, imIn2, imOut, se); 
        }
        RES_T operator()(const imageInType &imIn, const StrElt &se=DEFAULT_SE) 
        { 
            return this->_exec(imIn, se); 
        }
        
        virtual RES_T initialize(const imageInType &imIn, const imageInType &imIn2, imageOutType &imOut, const StrElt &se);
        virtual RES_T finalize(const imageInType &imIn, imageOutType &imOut, const StrElt &se);
      virtual RES_T _exec(const imageInType &imIn, const imageInType &imIn2, imageOutType &imOut, const StrElt &se);// BMI
        virtual RES_T _exec(const imageInType &imIn, const StrElt &se);
        
        virtual RES_T processImage(const imageInType &imIn, const imageInType &imIn2, imageOutType &imOut, const StrElt &se);
      virtual inline void processSlice(sliceInType linesIn,sliceInType linesIn2, sliceOutType linesOut, size_t &lineNbr, const StrElt &se);
      virtual inline void processLine(lineInType pixIn,lineInType pixIn2, lineOutType pixOut, size_t &pixNbr, const StrElt &se);
        virtual inline void processPixel(size_t pointOffset, vector<int> &dOffsets);
        
        static bool isInplaceSafe(const StrElt &/*se*/) { return false; }
        
        const Image<T_in> *imageIn;
        const Image<T_in> *imageIn2;
        Image<T_out> *imageOut;
        
    protected:
          size_t imSize[3];
          volInType slicesIn;
          volOutType slicesOut;
          lineInType pixelsIn;
          lineInType pixelsIn2;
          lineOutType pixelsOut;
          
          vector<IntPoint> sePoints;
          UINT sePointNbr;
          vector<int> relativeOffsets;
          
          int se_xmin;
          int se_xmax;
          int se_ymin;
          int se_ymax;
          int se_zmin;
          int se_zmax;

          bool oddSe;
    public:
        T_out initialValue;
        T_in borderValue;
    };

/** \} */

} // namespace smil

# endif // _MORPH_IMAGE_OPERATIONS2_HPP
