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


#include <stdio.h>
#include <time.h>


#include "Core/include/DCore.h"
#include "IO/include/private/DImageIO_RAW.hpp"
#include "Gui/include/DGui.h"

#include "Core/include/DColor.h"
#include "NSTypes/RGB/include/DRGB.h"


using namespace smil;

class Test_RW_RAW : public TestCase
{
  virtual void run()
  {
    typedef UINT8 T;
    const char *fName = "_smil_io_tmp.raw";
    
    Image<T> im1(3, 3, 2);
    Image<T> im2;
    
    T tab[] = { 28, 2, 3,
                 2, 5, 6,
                 3, 8, 9,
                 4, 11, 12,
                 5, 15, 16,
                 6, 18, 19 };
    im1 << tab;
    TEST_ASSERT( writeRAW(im1, fName)==RES_OK );
    TEST_ASSERT( readRAW(fName, 3,3,2, im2)==RES_OK );
    TEST_ASSERT(im1==im2);
  }
};

class Test_NetPBM : public TestCase
{
  virtual void run()
  {
    typedef UINT8 T;
    stringstream stream;
    
    Image<T> im1(3, 6);
    Image<T> im2;
    
    T tab_g[] = { 28, 2, 3,
                 2, 5, 6,
                 3, 8, 9,
                 4, 11, 12,
                 5, 15, 16,
                 6, 18, 19 };
    im1 << tab_g;
    
    PGM_FileHandler<T> gHandler;
    
    TEST_ASSERT( gHandler.write(im1, stream)==RES_OK );
    TEST_ASSERT( gHandler.read(stream, im2)==RES_OK );
    TEST_ASSERT(im1==im2);
    
    if (retVal!=RES_OK)
    {
        im1.printSelf(1);
        im2.printSelf(1);
    }
    
    T tab_b[] = { 255, 0, 255,
                 0, 0, 0,
                 0, 0, 255,
                 255, 255, 255,
                 0, 255, 0,
                 255, 0, 0 };
    im1 << tab_b;
    
    PBM_FileHandler<T> bHandler;
    
    TEST_ASSERT( bHandler.write(im1, stream)==RES_OK );
    TEST_ASSERT( bHandler.read(stream, im2)==RES_OK );
    TEST_ASSERT(im1==im2);
    
    if (retVal!=RES_OK)
    {
        im1.printSelf(1);
        im2.printSelf(1);
    }
    
    
    im1.load("http://cmm.ensmp.fr/~faessel/smil/images/lena.png");
  }
};

    
int main(int argc, char *argv[])
{
    typedef UINT8 T;
    Image<T> im1;
    BaseImageFileHandler *fp = new PGM_FileHandler<T>();
    
    fp->open("http://cmm.ensmp.fr/~faessel/smil/images/coins.pgm");
    fp->read(im1);
//     FileHandler fp("file://cmm.ensmp.fr/~faessel/smil/images/lena.png", ios_base::binary);
    
//     istream &is = fp.getStream();
    cout << fp->typeIsAvailable() << endl;
//     std::move(is);
  
    delete fp;
    
    TestSuite ts;

    ADD_TEST(ts, Test_RW_RAW);
    ADD_TEST(ts, Test_NetPBM);
    
    return ts.run();
}

