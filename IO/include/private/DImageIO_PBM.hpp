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
    
    RES_T readNetPBMFileInfo(istream &st, ImageFileInfo &fInfo);
    RES_T readNetPBMFileInfo(const char* filename, ImageFileInfo &fInfo);
    RES_T writeNetPBMFileInfo(ImageFileInfo &fInfo, ostream &ost);
    
    template <class T> class Image;

    template <class T, typename Enable=T>
    class PGMImageFileHandler : public ImageFileHandler<T>
    {
      public:
        PGMImageFileHandler()
          : ImageFileHandler<T>("PGM")
        {
        }
        
        virtual RES_T readHeader(istream &stream, ImageFileInfo &fInfo)
        {
            return readNetPBMFileInfo(stream, fInfo);
        }
    };

    template <class T>
    class PGMImageFileHandler<T, ENABLE_IF( IS_SAME(T, UINT8) , T ) >  
      : public PGMImageFileHandler<T,void>
    {
      public:
        
        virtual bool typeIsAvailable() { return true; }
        
        virtual RES_T read(istream &ist, Image<T> &image);
        virtual RES_T write(const Image<T> &image, ostream  &ost);
    };

    
    
    template <class T=void, typename Enable=T>
    class PBMImageFileHandler : public ImageFileHandler<T>
    {
      public:
        PBMImageFileHandler()
          : ImageFileHandler<T>("PGM")
        {
        }
        
        virtual RES_T getFileInfo(const char* filename, ImageFileInfo &fInfo)
        {
            return readNetPBMFileInfo(filename, fInfo);
        }
        
    };
    
    template <class T>  
    class PBMImageFileHandler< T, ENABLE_IF( IS_SAME(T, UINT8), T ) > 
      : public ImageFileHandler<T>
    {
      public:
        PBMImageFileHandler()
          : ImageFileHandler<T>("PBM")
        {
        }
        
        virtual RES_T getFileInfo(const char* filename, ImageFileInfo &fInfo)
        {
            return readNetPBMFileInfo(filename, fInfo);
        }
        
        virtual RES_T read(istream &ist, Image<T> &image)
        {
            ImageFileInfo fInfo;
            ASSERT(readNetPBMFileInfo(ist, fInfo)==RES_OK, RES_ERR_IO);
            ASSERT(fInfo.colorType==ImageFileInfo::COLOR_TYPE_BINARY, "Not an binary image", RES_ERR_IO);
            
            int width = fInfo.width;
            int height = fInfo.height;

            ASSERT((image.setSize(width, height)==RES_OK), RES_ERR_BAD_ALLOCATION);
            
            if (fInfo.fileType==ImageFileInfo::FILE_TYPE_BINARY)
            {
                typename ImDtTypes<T>::sliceType lines = image.getLines();
                
                char val;
                int k;
                
                for (size_t j=0;j<height;j++)
                {
                    typename ImDtTypes<T>::lineType pixels = lines[j];
                    
                    for (int i=0;i<width;i++)
                    {
                        if (i%8 == 0)
                          ist.read(&val, 1);
                        
                        k = 7 - i%8;
                        pixels[i] = ( ( val >> k )%2 )==0 ? T(0) : ImDtTypes<T>::max();
                    }
                }
            }
            else
            {
                typename ImDtTypes<T>::lineType pixels = image.getPixels();
                
                int val;
                for (size_t i=0;i<image.getPixelCount();i++)
                {
                    ist >> val;
                    pixels[i] = val==0 ? T(0) : ImDtTypes<T>::max();
                }
            }
            
            
            return RES_OK;
        }
        virtual RES_T write(const Image<T> &image, ostream &ost)
        {
            int width = image.getWidth();
            int height = image.getHeight();
            
            ImageFileInfo fInfo;
            fInfo.width = width;
            fInfo.height = height;
            fInfo.colorType = ImageFileInfo::COLOR_TYPE_BINARY;
            
            writeNetPBMFileInfo(fInfo, ost);
            
            char val;
            int k;
            typename ImDtTypes<T>::sliceType lines = image.getLines();
            
            for (size_t j=0;j<height;j++)
            {
                typename ImDtTypes<T>::lineType pixels = lines[j];
                val = 0;
                
                for (int i=0;i<width;i++)
                {
                    k = 7 - i%8;
                    
                    if (pixels[i]!=0)
                      val |= ( 1L << k );
                    
                    if (i>0 && i%8 == 0)
                    {
                        ost.write(&val, 1);
                        val = 0;
                    }
                }
                if (k!=7)
                  ost.write(&val, 1);
            }
            
            return RES_OK;
        }
    };

//     template <>
//     inline RES_T PBMImageFileHandler<void>::read(const char *, Image<void> &)
//     {
//         return RES_ERR;
//     }
// 
//     template <>
//     inline RES_T PBMImageFileHandler<void>::write(const Image<void> &, const char *)
//     {
//         return RES_ERR;
//     }
    
    // Specializations
//     template <>
//     RES_T PGMImageFileHandler<UINT8>::read(const char *filename, Image<UINT8> &image);
//     template <>
//     RES_T PGMImageFileHandler<UINT8>::write(const Image<UINT8> &image, const char *filename);

#ifdef SMIL_WRAP_RGB    
//     template <>
//     RES_T PGMImageFileHandler<RGB>::read(const char *filename, Image<RGB> &image);
//     template <>
//     RES_T PGMImageFileHandler<RGB>::write(const Image<RGB> &image, const char *filename);
#endif // SMIL_WRAP_RGB    
    
/*@}*/

} // namespace smil


#endif // _D_IMAGE_IO_PBM_H
