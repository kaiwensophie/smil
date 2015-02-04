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


#ifndef _D_IMAGE_FILE_MANAGER_HPP
#define _D_IMAGE_FILE_MANAGER_HPP


#include "IO/include/DFileHandler.h"
#include "Core/include/private/DTraits.hpp"



namespace smil
{
  
    /** 
    * \addtogroup IO
    */
    /*@{*/
    
    struct ImageFileHeader
    {
        ImageFileHeader()
        {
            reset();
        }
        template <class T>
        ImageFileHeader(const Image<T> &image)
        {
            getFromImage<T>(image);
        }
        
        template <class T>
        RES_T getFromImage(const Image<T> &image)
        {
            channels = 1;
            colorType = COLOR_TYPE_GRAY;
            scalarType = getDataTypeEquiv<T>();
            fileType = FILE_TYPE_BINARY;
            width = image.getWidth();
            height = image.getHeight();
            depth = image.getDepth();
            dataStartPos = 0;
            
            return RES_OK;
        }
        template <class T>
        RES_T applyToImage(Image<T> &image)
        {
            ASSERT(image.setSize(width, height, depth)==RES_OK, RES_ERR_BAD_ALLOCATION)
            
            return RES_OK;
        }
        
        enum ColorType { COLOR_TYPE_GRAY, COLOR_TYPE_RGB, COLOR_TYPE_GA, COLOR_TYPE_RGBA, COLOR_TYPE_BINARY, COLOR_TYPE_UNKNOWN };
        enum FileType { FILE_TYPE_ASCII, FILE_TYPE_BINARY };
        UINT channels;
        ColorType colorType;
        DType scalarType;
        FileType fileType;
        size_t width, height, depth;
        streampos dataStartPos;
        
        void reset()
        {
            channels = 1;
            colorType = COLOR_TYPE_UNKNOWN;
            scalarType = DtUNKNOWN;
            fileType = FILE_TYPE_BINARY;
            width = 0;
            height = 0;
            depth = 1;
            dataStartPos = 0;
        }
    };
    
    template <>
    inline RES_T ImageFileHeader::getFromImage<RGB>(const Image<RGB> &image)
    {
        channels = 3;
        colorType = COLOR_TYPE_RGB;
        scalarType = DtUINT8;
        fileType = FILE_TYPE_BINARY;
        width = image.getWidth();
        height = image.getHeight();
        depth = image.getDepth();
        dataStartPos = 0;
        
        return RES_OK;
    }
    
    class BaseImageFileHandler : public FileHandler
    {
    public:
        virtual ~BaseImageFileHandler() {}
        virtual RES_T readHeader() = 0;
        virtual RES_T writeHeader() = 0;
        virtual RES_T read(BaseImage &image) = 0;
        virtual RES_T read(iostream &fp, BaseImage &image) = 0;
        virtual RES_T read(const char *fileName, BaseImage &image) = 0;
        virtual RES_T write(const BaseImage &image) = 0;
        virtual RES_T write(const BaseImage &image, iostream &fp) = 0;
        virtual RES_T write(const BaseImage &image, const char *fileName)= 0;
        virtual bool typeIsAvailable() { return false; }
        
        ImageFileHeader header;
    };
    
    
    template <class T>
    class ImageFileHandler : public BaseImageFileHandler
    {
    public:
        virtual ~ImageFileHandler() {}
        virtual bool typeIsAvailable() { return false; }
        virtual RES_T readHeader() { Image<T> *im = NULL; return readHeader(*im); }
        virtual RES_T writeHeader() { Image<T> *im = NULL; return writeHeader(*im); }
        
        // Read the file header and apply it to an image if provided
        virtual RES_T readHeader(Image<T> &image=NULL)
        {
            if (&image)
              return this->header.applyToImage(image);
            else 
              return RES_OK;
        }
        // Write the file header (with data from an image if provided)
        virtual RES_T writeHeader(const Image<T> &image=NULL)
        {
            if (&image)
              return this->header.getFromImage(image);
            else 
              return RES_OK;
        }
        
        virtual RES_T readData(Image<T> &image) { return RES_ERR_NOT_IMPLEMENTED; }
        virtual RES_T writeData(const Image<T> &image) { return RES_ERR_NOT_IMPLEMENTED; }
        
        virtual RES_T read(BaseImage &image) 
        {
            return this->read(static_cast< Image<T>& >(image));
        }
        virtual RES_T read(iostream &fp, BaseImage &image) 
        {
            return this->read(fp, static_cast< Image<T>& >(image));
        }
        virtual RES_T read(const char *fileName, BaseImage &image) 
        {
            return this->read(fileName, static_cast< Image<T>& >(image));
        }
        virtual RES_T read(Image<T> &image) 
        {
            if (!typeIsAvailable())
            {
                cout << "Data type (" << getDataTypeAsString<T>() << ") not supported for ";
                cout << fileExtension << " files." << endl;
                return RES_ERR_NOT_IMPLEMENTED;
            }
            
            if (!this->_open && !this->stream)
            {
                ERR_MSG("No file/stream open.");
                return RES_ERR;
            }
            
            ASSERT(readHeader(image)==RES_OK)
            ASSERT(readData(image)==RES_OK)
            image.modified();
            
            if (this->_open)
              this->close();
            
            return RES_OK;
            
        }
        virtual RES_T read(iostream &fp, Image<T> &image) 
        {
            this->setStream(fp);
            return read(image);
        }
        virtual RES_T read(const char *fileName, Image<T> &image)
        {
            if (this->open(fileName, ios_base::in))
              return this->read(image);
            else return RES_ERR_IO;
        }
        
        virtual RES_T write(const BaseImage &image)
        {
            return this->write(static_cast< const Image<T>& >(image));
        }
        virtual RES_T write(const BaseImage &image, iostream &fp)
        {
            return this->write(static_cast< const Image<T>& >(image), fp);
        }
        virtual RES_T write(const BaseImage &image, const char *fileName)
        {
            return this->write(static_cast< const Image<T>& >(image), fileName);
        }
        virtual RES_T write(const Image<T> &image)
        {
            if (!typeIsAvailable())
            {
                cout << "Data type (" << getDataTypeAsString<T>() << ") not supported for ";
                cout << fileExtension << " files." << endl;
                return RES_ERR_NOT_IMPLEMENTED;
            }
            
            if (!this->_open && !this->stream)
            {
                ERR_MSG("No file/stream open.");
                return RES_ERR;
            }
            
            ASSERT(writeHeader(image)==RES_OK)
            ASSERT(writeData(image)==RES_OK)
            
            if (this->_open)
              this->close();
            
            return RES_OK;
        }
        virtual RES_T write(const Image<T> &image, iostream &fp) 
        {
            this->setStream(fp);
            return write(image);
        }
        virtual RES_T write(const Image<T> &image, const char *fileName)
        {
            if (this->open(fileName, ios_base::out))
              return this->write(image);
            else return RES_ERR_IO;
        }
    };


/*@}*/

} // namespace smil



#endif // _D_IMAGE_FILE_MANAGER_HPP
