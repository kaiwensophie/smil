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


#include "IO/include/DFileHandler.h"

namespace smil
{
  
   FileHandler::FileHandler()
      : stream(NULL), _open(false)
    {
    }
    
    FileHandler::FileHandler(const char* filename, ios_base::openmode mode)
      : stream(NULL), _open(false)
    {
        open(filename, mode);
    }
        
    FileHandler::~FileHandler()
    {
        close();
    }
    
    
    RES_T FileHandler::open(const char* filename,  ios_base::openmode mode)
    {
        fileName = filename;
        fileExtension = getFileExtension(fileName);
        string filePrefix = fileName.substr(0, 7);
        
        openMode = mode;
        if (!openMode & ios_base::out)
          openMode |= ios_base::in;
        
        if (filePrefix=="http://")
        {
    #ifndef USE_CURL
            ERR_MSG("Error: to use this functionality you must compile SMIL with the Curl option");
            return RES_ERR_IO;
    #else // USE_CURL
            if (openMode & ios_base::out)
            {
                ERR_MSG("Remote file access is only available in read mode.");
                return RES_ERR_IO;
            }
            
            stream = new stringstream();
            
            ASSERT(getHttpFile(filename, *stream)==RES_OK);
    #endif // USE_CURL
        }
        else
        {
            if (filePrefix=="file://")
              fileName = fileName.substr(7);

            stream = new fstream(fileName.c_str(), mode | ios_base::binary); // Force binary mode
        }
        
        _open = true;
        return RES_OK;
    }
    
    void FileHandler::close()
    {
        if (!_open)
          return;
        
        if (stream)
        {
          delete stream;
          stream = NULL;
        }
        
        _open = false;
    }
} // namespace smil

