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


#include "IO/include/DCommonIO.h"
#include "Core/include/DErrors.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <ctype.h>
#include <fstream>

#ifdef USE_CURL
#include <curl/curl.h>
#endif // USE_CURL

namespace smil
{


    string getFileExtension(string fileName)
    {
        string::size_type idx = fileName.rfind('.');
        string fExt = idx==string::npos ? "" : fileName.substr(idx+1).c_str();
        transform(fExt.begin(), fExt.end(), fExt.begin(), ::toupper);
        return fExt;
    }
    
    
#ifdef USE_CURL

    static size_t
    curlMemoryCallback(void *contents, size_t size, size_t nmemb, ostream *stream)
    {
        stream->write((const char*)contents, nmemb);

        return nmemb;
    }


    
    /**
    * Download file data into a string buffer.
    */
    RES_T getHttpFile(const char *url, ostream &buffer) 
    {
        CURL *curl_handle;
        CURLcode res;
        long respCode;
        
        curl_handle = curl_easy_init();
        
        if (curl_handle)
        {
            curl_easy_setopt(curl_handle, CURLOPT_URL, url);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curlMemoryCallback);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&buffer);
            res = curl_easy_perform(curl_handle);
            curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &respCode);
            curl_easy_cleanup(curl_handle);
        }
        else res = CURLE_FAILED_INIT;

        ASSERT(respCode==200, "File not found on server.", RES_ERR_IO)
        ASSERT((res==CURLE_OK), RES_ERR_IO)
        
        return RES_OK;
    }
    
    
    RES_T getHttpFile(const char *url, const char *outfilename) 
    {
        ofstream fp(outfilename, ios_base::binary);
        
        if (!fp.is_open())
        {
            cout << "Cannot open output file " << outfilename << endl;
            return RES_ERR_IO;
        }
        
        return getHttpFile(url, fp);
    }

#endif // USE_CURL
    
    
     
} // namespace smil

