// Copyright (c) 2011-2015, Matthieu FAESSEL and ARMINES
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

SMIL_MODULE(smilCore)


%{
#include "DImage.hxx"
%}

%include std_vector.i


//////////////////////////////////////////////////////////
// Init
//////////////////////////////////////////////////////////

%init
%{
%}


%import swig/smilCoreCommon.i

//////////////////////////////////////////////////////////
// Core Instance
//////////////////////////////////////////////////////////

%include "Core/include/DCpuID.h"
%include "Core/include/private/DInstance.hpp"
%template(CoreInstance) smil::UniqueInstance<Core>;
%include "Core/include/DCoreInstance.h"

#ifndef SWIGXML

namespace std 
{
    %template(ObjVector) vector<BaseObject*>;
}

#endif // SWIGXML

//////////////////////////////////////////////////////////
// Signals/Slots
//////////////////////////////////////////////////////////

#ifndef SWIGJAVA
// generate directors for Signal and Slot (for virtual methods overriding)
%feature("director") Signal;
%feature("director") Slot;
%feature("director") BaseImageEventSlot;
#endif // SWIGJAVA

%include "Core/include/DSignal.h"
%include "Core/include/DSlot.h"
%include "Core/include/DCoreEvents.h"


namespace smil
{
    %template(BaseImageEventSlot) Slot<BaseImageEvent>;
    %template(EventSlot) Slot<Event>;
    %template(ViewerFunctionSlot) MemberFunctionSlot<BaseImageViewer, Event>;
    %template(FunctionSlot_) FunctionSlot<Event>;
}

//////////////////////////////////////////////////////////
// Image
//////////////////////////////////////////////////////////

// Import smilGui for viewers stuff
%import smilGui.i


%ignore smil::Image::operator[];

#ifdef SWIGPYTHON
%ignore smil::Image::getPixels;
%ignore smil::Image::getLines;
%ignore smil::Image::getSlices;
%ignore smil::Image::toArray;
%ignore smil::Image::fromArray;
%ignore smil::Image::toCharArray;
%ignore smil::Image::fromCharArray;
%ignore smil::Image::toIntArray;
%ignore smil::Image::fromIntArray;

%extend smil::Image
{
    T __getitem__(size_t i) { return self->getPixelNoCheck(i); }
    void __setitem__(size_t i, T val) { return self->setPixelNoCheck(i, val); }
}
#endif // SWIGPYTHON

%feature("new","0") castBaseImage;
%include "Core/include/DBaseImage.h"
%include "Core/include/private/DImage.hpp"
%include "Core/include/private/DSharedImage.hpp"

#ifndef SWIGXML

namespace std 
{
    %template(ImgVector) std::vector<BaseImage*>;
}

#endif // SWIGXML

namespace smil
{
    TEMPLATE_WRAP_CLASS(Image, Image);
    TEMPLATE_WRAP_CLASS(ResImage, ResImage);
    TEMPLATE_WRAP_FUNC(createImage);
    TEMPLATE_WRAP_FUNC(castBaseImage);
    TEMPLATE_WRAP_CLASS(SharedImage, SharedImage);
    TEMPLATE_WRAP_FUNC(drawOverlay);
    
    TEMPLATE_WRAP_SUPPL_CLASS(Image, Image);
    TEMPLATE_WRAP_SUPPL_FUNC(createImage);
    TEMPLATE_WRAP_SUPPL_FUNC(castBaseImage);
    TEMPLATE_WRAP_SUPPL_CLASS(SharedImage, SharedImage);
}


//////////////////////////////////////////////////////////
// Misc
//////////////////////////////////////////////////////////

%{
#include "DGraph.hpp"
%}





#ifndef SMIL_WRAP_UINT32
    %template(Vector_Edge_UINT) std::vector< Edge<UINT,UINT> >;
    TEMPLATE_WRAP_CLASS_2T_SUBTYPES_FIX_FIRST(std::vector, Edge, UINT, Vector);
    TEMPLATE_WRAP_CLASS_2T_SUBTYPES_FIX_SECOND(std::vector, Edge, UINT, Vector);
#endif // SMIL_WRAP_UINT32
    
%include "Core/include/private/DGraph.hpp"


#ifndef SWIGXML
    
    // Base UINT Edge
    TEMPLATE_WRAP_CLASS_2T_CROSS(smil::Edge, Edge);
    
    TEMPLATE_WRAP_CLASS_2T_SUBTYPES_CROSS(vector, Edge, Vector);
    
#ifndef SMIL_WRAP_UINT32
    
    TEMPLATE_WRAP_CLASS_2T_FIX_FIRST(Edge, UINT, Edge);
    TEMPLATE_WRAP_CLASS_2T_FIX_SECOND(Edge, UINT, Edge);
    
#endif // SMIL_WRAP_UINT32

#endif // SWIGXML

namespace smil
{
    // Graph & MST
    %template(Graph_SIZE_T) Graph<size_t,size_t>;
    %template(graphMST_SIZE_T) graphMST<Graph<UINT,UINT> >;
    
    TEMPLATE_WRAP_CLASS_2T_CROSS(Graph, Graph);
    
    #if !defined(SMIL_WRAP_UINT32) && !defined(SMIL_WRAP_UINT) 
        %template(Graph_UINT) Graph<UINT,UINT>;
        %template(graphMST_UINT) graphMST<Graph<UINT,UINT> >;

        TEMPLATE_WRAP_CLASS_2T_FIX_FIRST(Graph, UINT, Graph);
        TEMPLATE_WRAP_CLASS_2T_FIX_SECOND(Graph, UINT, Graph);
    #endif

}



//////////////////////////////////////////////////////////
// Internal definitions 
// (not visible by other modules)
//////////////////////////////////////////////////////////
#ifndef SWIGIMPORTED

    // Numpy
    #if defined SWIGPYTHON && defined USE_NUMPY
    %init
    %{
        // Required by NumPy in Python initialization
        import_array();
    %}
    %{
    #include "Core/include/private/DNumpyInterface.hpp"
    %}

    %include "Core/include/private/DNumpyInterface.hpp"

    TEMPLATE_WRAP_CLASS(NumpyInt,NumpyInt)

%pythoncode %{


def NumpyInt(array):
    """
    * Create a SharedImage interface with a NumPy array
    """

    if str(type(array))!="<type 'numpy.ndarray'>":
      print("You must specify a NumPy array")
      return
    
    dt = array.dtype.name
    
    if dt=='uint8':
      return NumpyInt_UINT8(array)
    elif dt=='uint16':
      return NumpyInt_UINT16(array)
    elif dt=='uint32':
      return NumpyInt_UINT32(array)
%}

        
    #endif // defined SWIGPYTHON && defined USE_NUMPY
#endif // SWIGIMPORTED

