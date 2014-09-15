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


#include "Core/include/DBaseObject.h"
#include "Core/include/DBaseImage.h"
#include "Core/include/DCoreInstance.h"
#include "DGui.h"
#include "Core/include/DCpuID.h"

#ifdef USE_OPEN_MP
#include <omp.h>
#endif // USE_OPEN_MP



using namespace smil;

Core::Core ()
// : BaseObject("Core", false),
  : keepAlive(false),
    autoResizeImages(true),
    threadNumber(1),
    maxThreadNumber(1),
    systemName(SYSTEM_NAME),
    targetArchitecture(TARGET_ARCHITECTURE),
#ifdef USE_OPEN_MP
    supportOpenMP(true)
#else // USE_OPEN_MP
    supportOpenMP(false)
#endif // USE_OPEN_MP
{
#ifdef USE_OPEN_MP
    maxThreadNumber = cpuID.getLogical();
    coreNumber = cpuID.getCores();
    threadNumber = coreNumber;
#else // USE_OPEN_MP
    maxThreadNumber = 1;
    coreNumber = cpuID.getCores();
    threadNumber = 1;
#endif // USE_OPEN_MP
}

Core::~Core ()
{
    deleteRegisteredObjects();
#if DEBUG_LEVEL > 1
      cout << "Core deleted" << endl;
#endif // DEBUG_LEVEL > 1
}


void Core::registerObject(BaseObject *obj)
{
    if (obj->registered)
      return;

    Core *inst = Core::getInstance();
    inst->registeredObjects.push_back(obj);

    obj->registered = true;

    if (string(obj->getClassName())=="Image")
	inst->registeredImages.push_back(static_cast<BaseImage*>(obj));

#if DEBUG_LEVEL > 1
    cout << "Core::registerObject: " << obj->getClassName() << " " << obj << " created." << endl;
#endif // DEBUG_LEVEL > 1
}


void Core::unregisterObject(BaseObject *obj)
{
    if (!obj->registered)
      return;

    Core *inst = Core::getInstance();
    inst->registeredObjects.erase(std::remove(inst->registeredObjects.begin(), inst->registeredObjects.end(), obj));

    obj->registered = false;

    if (string(obj->getClassName())=="Image")
	inst->registeredImages.erase(std::remove(inst->registeredImages.begin(), inst->registeredImages.end(), static_cast<BaseImage*>(obj)));

#if DEBUG_LEVEL > 1
    cout << "Core::unregisterObject: " << obj->getClassName() << " " << obj << " deleted." << endl;
#endif // DEBUG_LEVEL > 1

    if (!inst->keepAlive && inst->registeredObjects.size()==0)
	inst->kill();
}


void Core::deleteRegisteredObjects()
{
    vector<BaseObject*> objects = registeredObjects;

    for (UINT i=0;i<objects.size();i++)
      delete objects[i];
}

UINT Core::getNumberOfThreads()
{
    return this->threadNumber;
}

UINT Core::getNumberOfCores()
{
    return this->threadNumber;
}

UINT Core::getMaxNumberOfThreads()
{
    return this->maxThreadNumber;
}

RES_T Core::setNumberOfThreads(UINT nbr)
{
    ASSERT((nbr<=maxThreadNumber), "Nbr of thread exceeds system capacity !", RES_ERR);
    this->threadNumber = nbr;
    return RES_OK;
}

void Core::resetNumberOfThreads()
{
    this->threadNumber = this->maxThreadNumber;
}


size_t Core::getAllocatedMemory()
{
    vector<BaseImage*>::iterator it = this->registeredImages.begin();
    size_t totAlloc = 0;

    while (it!=this->registeredImages.end())
	totAlloc += (*it++)->getAllocatedSize();
    return totAlloc;
}

vector<BaseObject*> Core::getRegisteredObjects() 
{ 
    return this->registeredObjects; 
}

vector<BaseImage*> Core::getImages()  
{ 
    return this->registeredImages; 
}

void Core::showAllImages()
{
    vector<BaseImage*>::iterator it = this->registeredImages.begin();

    while (it!=this->registeredImages.end())
	(*it++)->show();
}

void Core::hideAllImages()
{
    vector<BaseImage*>::iterator it = this->registeredImages.begin();

    while (it!=this->registeredImages.end())
	(*it++)->hide();
}

void Core::deleteAllImages()
{
    vector<BaseImage*> imgs = this->registeredImages;
    for (int i=0;i<imgs.size();i++)
      delete imgs[i];
}

void Core::getCompilationInfos(ostream &outStream)
{
    outStream << "Build date: " << __DATE__ << " (" << __TIME__ << ")" << endl;
#ifdef DEBUG    
    outStream << "Build type: debug"<< endl;
#else
    outStream << "Build type: release"<< endl;
#endif
    outStream << "System: " << this->systemName << endl;
    outStream << "Target Architecture: " << this->targetArchitecture << endl;
    outStream << "OpenMP support: " << (this->supportOpenMP ? "On" : "Off") << endl;
}


