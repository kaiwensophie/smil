// Copyright (c) 2011, Matthieu FAESSEL and ARMINES
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Matthieu FAESSEL, or ARMINES nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.




%include smilCommon.i

SMIL_MODULE(smilBase)


//////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////

%{
/* Includes the header in the wrapper code */
#include "DImage.hxx"
#include "DImageArith.hpp"
#include "DImageDraw.hpp"
#include "DImageHistogram.hpp"
#include "DImageTransform.hpp"
#include "DBaseMeasureOperations.hpp"
#include "DMeasures.hpp"
#include "DImageMatrix.hpp"
#include "DLabelMeasures.hpp"

#include <stdexcept>

%}

// Import smilCore to have correct function signatures (arguments with Image_UINT8 instead of Image<unsigned char>)
%import "smilCore.i"

PTR_ARG_OUT_APPLY(ret_min)
PTR_ARG_OUT_APPLY(ret_max)
PTR_ARG_OUT_APPLY(w)
PTR_ARG_OUT_APPLY(h)
PTR_ARG_OUT_APPLY(d)

%include "DImageArith.hpp"
%include "DImageDraw.hpp"
%include "DImageTransform.hpp"


TEMPLATE_WRAP_FUNC_CROSS2(copy);
TEMPLATE_WRAP_FUNC(crop);
TEMPLATE_WRAP_FUNC(clone);

TEMPLATE_WRAP_FUNC(inv);
TEMPLATE_WRAP_FUNC(fill);
TEMPLATE_WRAP_FUNC(randFill);
TEMPLATE_WRAP_FUNC(add);
TEMPLATE_WRAP_FUNC(addNoSat);
TEMPLATE_WRAP_FUNC(sub);
TEMPLATE_WRAP_FUNC(subNoSat);
TEMPLATE_WRAP_FUNC(mul);
TEMPLATE_WRAP_FUNC(mulNoSat);
TEMPLATE_WRAP_FUNC(div);

TEMPLATE_WRAP_FUNC(equ);
TEMPLATE_WRAP_FUNC(diff);
TEMPLATE_WRAP_FUNC(absDiff);
TEMPLATE_WRAP_FUNC(sup);
TEMPLATE_WRAP_FUNC(inf);
TEMPLATE_WRAP_FUNC(low);
TEMPLATE_WRAP_FUNC(lowOrEqu);
TEMPLATE_WRAP_FUNC(grt);
TEMPLATE_WRAP_FUNC(grtOrEqu);
TEMPLATE_WRAP_FUNC(logicAnd);
TEMPLATE_WRAP_FUNC(logicOr);
TEMPLATE_WRAP_FUNC(logicXOr);
TEMPLATE_WRAP_FUNC(bitAnd);
TEMPLATE_WRAP_FUNC(bitOr);
TEMPLATE_WRAP_FUNC(bitXOr);
TEMPLATE_WRAP_FUNC(test);
TEMPLATE_WRAP_FUNC(compare);
TEMPLATE_WRAP_FUNC(mask);
TEMPLATE_WRAP_FUNC_CROSS2(applyLookup);



%include "DImageHistogram.hpp"
TEMPLATE_WRAP_FUNC(histogram);
TEMPLATE_WRAP_FUNC(threshold);
TEMPLATE_WRAP_FUNC_CROSS2(stretchHist);
TEMPLATE_WRAP_FUNC(enhanceContrast);
TEMPLATE_WRAP_FUNC(otsuThresholdValues);
TEMPLATE_WRAP_FUNC(otsuThreshold);


TEMPLATE_WRAP_FUNC(drawLine);
TEMPLATE_WRAP_FUNC(drawRectangle);
TEMPLATE_WRAP_FUNC(drawBox);
#ifdef USE_FREETYPE
TEMPLATE_WRAP_FUNC(drawText);
#endif // USE_FREETYPE

TEMPLATE_WRAP_FUNC(vFlip);
TEMPLATE_WRAP_FUNC(trans);
TEMPLATE_WRAP_FUNC(resize);
TEMPLATE_WRAP_FUNC(scale);

%include "DBaseMeasureOperations.hpp"


// Weird swig error...
%{
#ifndef SWIGPY_SLICE_ARG
#define SWIGPY_SLICE_ARG(obj) ((PySliceObject*) (obj))
#endif // SWIGPY_SLICE_ARG
%}

namespace std 
{
    %template(PixelSequenceVector) vector<PixelSequence>;
    %template(BlobMap) map<UINT,Blob>;
}

TEMPLATE_WRAP_FUNC(computeBlobs);

%include "DMeasures.hpp"
TEMPLATE_WRAP_FUNC(vol);
TEMPLATE_WRAP_FUNC(meanVal);
TEMPLATE_WRAP_FUNC(area);
TEMPLATE_WRAP_FUNC(minVal);
TEMPLATE_WRAP_FUNC(maxVal);
TEMPLATE_WRAP_FUNC(rangeVal);
TEMPLATE_WRAP_FUNC(measBarycenter);
TEMPLATE_WRAP_FUNC(measBoundBox);
TEMPLATE_WRAP_FUNC(measInertiaMatrix);
TEMPLATE_WRAP_FUNC(nonZeroOffsets);

%include "DLabelMeasures.hpp"
TEMPLATE_WRAP_FUNC(measAreas);
TEMPLATE_WRAP_FUNC(measMinVals);
TEMPLATE_WRAP_FUNC(measMaxVals);
TEMPLATE_WRAP_FUNC(measRangeVals);
TEMPLATE_WRAP_FUNC(measMeanVals);
TEMPLATE_WRAP_FUNC(measVolumes);
TEMPLATE_WRAP_FUNC(measBarycenters);
TEMPLATE_WRAP_FUNC(measBoundBoxes);
TEMPLATE_WRAP_FUNC(measInertiaMatrices);

%include "DImageMatrix.hpp"
TEMPLATE_WRAP_FUNC(matMul);


#ifndef SWIGIMPORTED
%include "smilCommonEnd.i"
#endif
