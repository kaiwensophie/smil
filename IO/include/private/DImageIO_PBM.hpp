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
    
    // Portable GrayMap
    template <class T, typename Enable=T>
    class PGM_FileHandler : public ImageFileHandler<T>
    {
      public:
        virtual RES_T readHeader(Image<T> &image=NULL)
        {
            ASSERT(readNetPBMHeader(this->getStream(), this->header)==RES_OK)
            
            return ImageFileHandler<T>::readHeader(image); // Apply header to image
        }
        virtual RES_T writeHeader(const Image<T> &image=NULL)
        {
            ASSERT(ImageFileHandler<T>::writeHeader(image)==RES_OK) // Get header infos from image
            
            if (&image)
              imageMaxVal = maxVal(image);
            
            return writeNetPBMHeader(this->header, this->fileName.c_str(), imageMaxVal, this->getStream());
        }
        
        int imageMaxVal;
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
    
    // Portable BitMap
    template <class T>
    class PBM_FileHandler<T> : public ImageFileHandler<T>
    {
      public:
        virtual bool typeIsAvailable() { return true; }

        virtual RES_T readHeader(Image<T> &image=NULL)
        {
            ASSERT(readNetPBMHeader(this->getStream(), this->header)==RES_OK)
            
            return ImageFileHandler<T>::readHeader(image); // Apply header to image
        }
        virtual RES_T writeHeader(const Image<T> &image=NULL)
        {
            ASSERT(ImageFileHandler<T>::writeHeader(image)==RES_OK) // Get header infos from image
            
            this->header.colorType = ImageFileHeader::COLOR_TYPE_BINARY;
            
            return writeNetPBMHeader(this->header, this->fileName.c_str(), 0, this->getStream());
        }
        
        virtual RES_T readData(Image<T> &image)
        {
            istream &fp = this->getStream();
            ImageFileHeader &header = this->header;

            ASSERT(header.colorType==ImageFileHeader::COLOR_TYPE_BINARY, "Not an binary image", RES_ERR_IO)
            
            int width = header.width;
            int height = header.height;

            if (header.fileType==ImageFileHeader::FILE_TYPE_BINARY)
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
                          fp.read(&val, 1);
                        
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
                    fp >> val;
                    pixels[i] = val==0 ? T(0) : ImDtTypes<T>::max();
                }
            }
            
            return RES_OK;
        }
        
        virtual RES_T writeData(const Image<T> &image)
        {
            ostream &fp = this->getStream();
            ImageFileHeader &header = this->header;

            int width = header.width;
            int height = header.height;

            typename ImDtTypes<T>::sliceType lines = image.getLines();
            
            if (header.fileType==ImageFileHeader::FILE_TYPE_BINARY)
            {
                
                char val;
                int k;
                
                for (size_t j=0;j<height;j++)
                {
                    typename ImDtTypes<T>::lineType pixels = lines[j];
                    val = 0;
                    
                    for (int i=0;i<width;i++)
                    {
                        k = 7 - i%8;
                        
                        if (pixels[i]!=T(0))
                          val |= (1 << k);
                        
                        if (i>0 && i%8 == 0)
                        {
                            fp.write(&val, 1);
                            val = 0;
                        }
                    }
                    if (k!=7)
                      fp.write(&val, 1);
                }
            }
            else
            {
                typename ImDtTypes<T>::lineType pixels = image.getPixels();
                
                int val;
                for (size_t j=0;j<height;j++)
                {
                    typename ImDtTypes<T>::lineType pixels = lines[j];
                    
                    for (int i=0;i<width;i++)
                    {
                        val = pixels[i]==T(0) ? 0 : 1;
                        fp << val << " ";
                    }
                    fp << endl;
                }
            }
            
            return RES_OK;
        }
        
    };    
    
    
/*@}*/

} // namespace smil


#endif // _D_IMAGE_IO_PBM_H
