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


#ifndef _D_IMAGE_IO_PBM_H
#define _D_IMAGE_IO_PBM_H

#include <fstream>
#include <iostream> 

#include "Core/include/private/DTypes.hpp"
#include "Core/include/private/DImage.hpp"
#include "IO/include/private/DImageIO.hpp"

namespace smil
{
    /** 
    * \addtogroup IO
    */
    /*@{*/

    RES_T readNetPBMHeader(istream &st, ImageFileHeader &header);
    RES_T writeNetPBMHeader(ImageFileHeader &header, const char *fileName, const int maxValue, ostream &ost);
    
    template <class T, typename Enable=T>
    class PGM_FileHandler : public ImageFileHandler<T>
    {
      public:
        virtual RES_T readHeader()
        {
            return readNetPBMHeader(this->getStream(), this->header);
        }
    };    
    
    template <class T>
    class PGM_FileHandler<T, ENABLE_IF( IS_SAME(T, UINT8), T )> : public PGM_FileHandler<T,void>
    {
      public:
        virtual bool typeIsAvailable() { return true; }

        virtual RES_T readData(Image<T> &image);
        virtual RES_T writeData(const Image<T> &image);
    };    
    
    template <class T, typename Enable=T>
    class PBM_FileHandler : public ImageFileHandler<T>
    {
      public:
        virtual RES_T readHeader()
        {
            return readNetPBMHeader(this->getStream(), this->header);
        }
    };    
    
    template <class T>
    class PBM_FileHandler<T, ENABLE_IF( IS_SAME(T, UINT8), T )> : public PGM_FileHandler<T,void>
    {
      public:
        virtual bool typeIsAvailable() { return true; }

//         virtual RES_T readData(Image<T> &image);
//         virtual RES_T writeData(const Image<T> &image);
    };    
    
    
/*@}*/

} // namespace smil


#endif // _D_IMAGE_IO_PBM_H
