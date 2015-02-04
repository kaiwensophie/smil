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


#ifndef _D_IMAGE_IO_BMP_H
#define _D_IMAGE_IO_BMP_H

#include <fstream>
#include <iostream> 

#include "IO/include/private/DImageIO.hpp"
#include "Core/include/private/DImage.hpp"

namespace smil
{
    /** 
    * \addtogroup IO
    */
    /*@{*/
    
    
    RES_T readBMPHeader(istream &fp, ImageFileHeader &header);
    RES_T writeBMPHeader(ImageFileHeader &header, ostream &fp);
    
    template <class T, typename Enable=T>
    class BMP_FileHandler : public ImageFileHandler<T>
    {
      public:
        
        virtual RES_T readHeader(Image<T> &image=NULL)
        {
            ASSERT(readBMPHeader(this->getStream(), this->header)==RES_OK)
            
            return ImageFileHandler<T>::readHeader(image); // Apply header to image
        }
        virtual RES_T writeHeader(const Image<T> &image=NULL)
        {
            ASSERT(ImageFileHandler<T>::writeHeader(image)==RES_OK) // Get header infos from image
            return writeBMPHeader(this->header, this->getStream());
        }
        
        
    };

    template <class T>
    class BMP_FileHandler<T, ENABLE_IF( IS_SAME(T, UINT8) || IS_SAME(T, RGB) , T )> : public BMP_FileHandler<T,void>
    {
      public:
        virtual bool typeIsAvailable() { return true; }

        virtual RES_T readData(Image<T> &image);
        virtual RES_T writeData(const Image<T> &image);
    };    
    
    
/*@}*/

} // namespace smil


#endif // _D_IMAGE_IO_BMP_H
