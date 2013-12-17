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


#ifndef _D_IMAGE_IO_H
#define _D_IMAGE_IO_H

#include "Core/include/DErrors.h"
#include "Core/include/DColor.h"
#include "Core/include/private/DImage.hpp"

namespace smil
{
  
    /** 
    * \addtogroup IO
    */
    /*@{*/
    
    string getFileExtension(const char *fileName);

    class FileCloser
    {
    public:
	FileCloser(FILE *_fp)
	{
	    fp = _fp;
	}
	~FileCloser()
	{
	    if (fp)
	      fclose(fp);
	}
    protected:
	FILE *fp;
    };

    struct ImageFileInfo
    {
	ImageFileInfo()
	  : width(0), height(0), depth(0),
	    colorType(COLOR_TYPE_UNKNOWN), scalarType(SCALAR_TYPE_UNKNOWN)
	{
	}
	enum ColorType { COLOR_TYPE_GRAY, COLOR_TYPE_RGB, COLOR_TYPE_GA, COLOR_TYPE_RGBA, COLOR_TYPE_UNKNOWN };
	enum ScalarType { SCALAR_TYPE_UINT8, SCALAR_TYPE_UINT16, SCALAR_TYPE_INT8, SCALAR_TYPE_INT16, SCALAR_TYPE_FLOAT, SCALAR_TYPE_DOUBLE, SCALAR_TYPE_UNKNOWN };
	UINT channels;
	ColorType colorType;
	ScalarType scalarType;
	size_t width, height, depth;
    };
    
    class BaseImage;
    template<class T> class Image;
    
    template <class T=void>
    class ImageFileHandler
    {
    public:
      ImageFileHandler(const char *ext)
	: fileExtention(ext)
      {
      }
      const char *fileExtention;
      
      virtual RES_T getFileInfo(const char* filename, ImageFileInfo &fInfo) 
      { 
	  return RES_ERR; 
      }
      
      virtual RES_T read(const char* filename)
      {
	  return RES_ERR;
      }
      
      virtual RES_T read(const char* filename, Image<T> &image)
      {
	  cout << "Data type not implemented for " << fileExtention << " files." << endl;
	  return RES_ERR;
      }
      virtual RES_T write(const Image<T> &image, const char* filename)
      {
	  cout << "Data type not implemented for " << fileExtention << " files." << endl;
	  return RES_ERR;
      }
    };
    
    #ifdef USE_CURL

    RES_T getHttpFile(const char *url, const char *outfilename) ;

    #endif // USE_CURL
/*@}*/

} // namespace smil



#endif // _D_IMAGE_IO_HPP
