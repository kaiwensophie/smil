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

#include "DGpu.h"

#include <iostream>


using namespace smil;
using namespace std;


GpuContext::GpuContext()
  : platformCount(0)
{
    clGetPlatformIDs (0, NULL, &platformCount);

    if (platformCount == 0) 
    {
	    cerr << "No OpenCL platform found" << endl;
	    enabled = false;
	    return;
    }
    
    platformIds.resize(platformCount);
    clGetPlatformIDs(platformCount, platformIds.data(), NULL);
    
}

GpuContext::~GpuContext()
{
    if (!enabled)
      return;
    
    
}

string GpuContext::getPlatformName(uint platformId)
{
    size_t size = 0;
    cl_platform_id id = platformIds[platformId];
    clGetPlatformInfo (id, CL_PLATFORM_NAME, 0, NULL, &size);

    std::string name;
    name.resize (size);
    clGetPlatformInfo (id, CL_PLATFORM_NAME, size, const_cast<char*> (name.data()), NULL);

    return name;
}

string GpuContext::getDeviceName(uint deviceId, uint platformId)
{
    size_t size = 0;
    cl_device_id id = deviceIds[platformId][deviceId];
    clGetDeviceInfo (id, CL_DEVICE_NAME, 0, NULL, &size);

    std::string name;
    name.resize(size);
    clGetDeviceInfo (id, CL_DEVICE_NAME, size, const_cast<char*> (name.data()), NULL);

    return name;
}
