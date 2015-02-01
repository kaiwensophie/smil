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


#include "Core/include/DErrors.h"
#include "IO/include/private/DImageIO_PBM.hpp"
#include "IO/include/private/DImageIO.hpp"
#include "Core/include/private/DImage.hpp"
#include "Core/include/DColor.h"
#include "Base/include/private/DMeasures.hpp"

namespace smil
{
  
    RES_T readNetPBMHeader(istream &fp, ImageFileHeader &header)
    {
        std::string buf;

        getline(fp, buf);
        
        if ( (buf[0]!='P' && buf[0]!='p'))
        {
            ERR_MSG("Error reading NetPBM header");
            return RES_ERR_IO;
        }
        
        int pbmFileType;
        pbmFileType = atoi(buf.data()+1);
        
        switch(pbmFileType)
        {
            case 1: // Portable BitMap ASCII
            header.colorType = ImageFileHeader::COLOR_TYPE_BINARY;
            header.fileType = ImageFileHeader::FILE_TYPE_ASCII;
            header.channels = 1;
            break;
            case 2: // Portable GrayMap ASCII
            header.colorType = ImageFileHeader::COLOR_TYPE_GRAY;
            header.fileType = ImageFileHeader::FILE_TYPE_ASCII;
            header.channels = 1;
            break;
            case 3: // Portable PixMap ASCII
            header.colorType = ImageFileHeader::COLOR_TYPE_RGB;
            header.fileType = ImageFileHeader::FILE_TYPE_ASCII;
            header.channels = 3;
            break;
            case 4: // Portable BitMap ASCII
            header.colorType = ImageFileHeader::COLOR_TYPE_BINARY;
            header.fileType = ImageFileHeader::FILE_TYPE_BINARY;
            header.channels = 1;
            break;
            case 5: // Portable GrayMap ASCII
            header.colorType = ImageFileHeader::COLOR_TYPE_GRAY;
            header.fileType = ImageFileHeader::FILE_TYPE_BINARY;
            header.channels = 1;
            break;
            case 6: // Portable PixMap ASCII
            header.colorType = ImageFileHeader::COLOR_TYPE_RGB;
            header.fileType = ImageFileHeader::FILE_TYPE_BINARY;
            header.channels = 3;
            break;
            default:
            ERR_MSG("Unknown NetPBM format");
            return RES_ERR_IO;
        }
        
        streampos curpos;
        // Read comments
        do
        {
            curpos = fp.tellg();
            getline(fp, buf);
        } while(buf[0]=='#');
        
        // Read image dimensions
        fp.seekg(curpos);
        fp >> header.width >> header.height;
        
        if (header.colorType!=ImageFileHeader::COLOR_TYPE_BINARY)
        {
            int dum;
            fp >> dum; // Max pixel value
        }
        
        fp.seekg(1, ios_base::cur); // endl
        
        header.dataStartPos = fp.tellg();
        header.scalarType = ImageFileHeader::SCALAR_TYPE_UINT8;
        
        return RES_OK;
    }
    
    
    RES_T writeNetPBMHeader(ImageFileHeader &header, const char *fileName, const int maxValue, ostream &ost)
    {
        ost << "P";
        
        switch(header.colorType)
        {
          case ImageFileHeader::COLOR_TYPE_BINARY:
            ost << "4" << endl;
            break;
          case ImageFileHeader::COLOR_TYPE_GRAY:
            ost << "5" << endl;
            break;
          case ImageFileHeader::COLOR_TYPE_RGB:
            ost << "6" << endl;
            break;
        }
        ost << "# " << fileName << endl;
        ost << header.width << " " << header.height << endl;
        
        // Max pixel val
        if (header.colorType!=ImageFileHeader::COLOR_TYPE_BINARY)
          ost << maxValue << endl;
        
        return RES_OK;
    }
    
    template <>
    RES_T PGM_FileHandler<UINT8>::readData(Image<UINT8> &image)
    {
        istream &fp = this->getStream();
        ImageFileHeader &header = this->header;
        
        ASSERT(this->header.colorType==ImageFileHeader::COLOR_TYPE_GRAY, "Not an 8bit gray image", RES_ERR_IO);
        
        int width = header.width;
        int height = header.height;

        ASSERT((image.setSize(width, height)==RES_OK), RES_ERR_BAD_ALLOCATION);
        
        if (header.fileType==ImageFileHeader::FILE_TYPE_BINARY)
        {
            fp.read((char*)image.getPixels(), width*height);
        }
        else
        {
            ImDtTypes<UINT8>::lineType pixels = image.getPixels();
            
            for (size_t i=0;i<image.getPixelCount();i++, pixels++)
              fp >> *((int*)pixels);
        }
        
        return RES_OK;
    }


    template <>
    RES_T PGM_FileHandler<UINT8>::writeData(const Image<UINT8> &image)
    {
        ostream &fp = this->getStream();
        
        size_t width = image.getWidth(), height = image.getHeight();
        
        fp << "P5" << endl;
        fp << "# " << image.getName() << endl;
        fp << width << " " << height << endl;
        fp << (int)maxVal(image) << endl;
        
        fp.write((char*)image.getPixels(), width*height);
        
        return RES_OK;
    }
    
    
#ifdef SMIL_WRAP_RGB      
#endif // SMIL_WRAP_RGB  

} // namespace smil
