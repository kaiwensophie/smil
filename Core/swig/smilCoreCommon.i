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

SMIL_MODULE(smilCoreCommon)



//////////////////////////////////////////////////////////
// Errors
//////////////////////////////////////////////////////////

%ignore Error;
%include "Core/include/DErrors.h"



//////////////////////////////////////////////////////////
// Types
//////////////////////////////////////////////////////////


%include "Core/include/private/DTypes.hpp"



// BitArray
#ifdef SMIL_WRAP_BIT
%include "Bit.i"
#endif // SMIL_WRAP_BIT

// RGB
#ifdef SMIL_WRAP_RGB
%include "RGB.i"
#else
%include "include/DColor.h"
#endif // SMIL_WRAP_RGB


//////////////////////////////////////////////////////////
// Typemaps
//////////////////////////////////////////////////////////

%rename(Index) operator[](UINT i);

PTR_ARG_OUT_APPLY(ret_min)
PTR_ARG_OUT_APPLY(ret_max)
PTR_ARG_OUT_APPLY(w)
PTR_ARG_OUT_APPLY(h)
PTR_ARG_OUT_APPLY(d)
PTR_ARG_OUT_APPLY(s)



//////////////////////////////////////////////////////////
// BaseObject
//////////////////////////////////////////////////////////

%extend smil::BaseObject 
{
        std::string  __str__() 
        {
            std::stringstream os;
            self->printSelf(os);
            return os.str();
        }
}


%include "Core/include/DCommon.h"
%include "Core/include/DBaseObject.h"

%template(DoublePoint) Point<double>;
%template(IntPoint) Point<int>;
%template(UintPoint) Point<UINT>;
%template(Size_tPoint) Point<size_t>;


//////////////////////////////////////////////////////////
// Vectors
//////////////////////////////////////////////////////////

#ifndef SWIGXML


%include std_vector.i
%include std_set.i

// Expose std::vector<> as a Python list
namespace std 
{
    TEMPLATE_WRAP_CLASS(vector, Vector);
    
#ifdef USE_64BIT_IDS
    %template(Vector_size_t) vector<size_t>;
#endif // USE_64BIT_IDS
    %template(Vector_int) vector<int>;
    %template(Vector_double) vector<double>;
    %template(Vector_string) vector<string>;
    
    %template(Matrix_double) vector<Vector_double>;
    %template(Vector_IntPoint) vector< smil::Point<int> >;

    TEMPLATE_WRAP_CLASS(set, Set);
    
#if !defined(SMIL_WRAP_UINT32) && !defined(SMIL_WRAP_UINT)
    %template(Vector_UINT) vector<UINT>;
    %template(Set_UINT) set<UINT>;
#endif // !defined(SMIL_WRAP_UINT32) && !defined(SMIL_WRAP_UINT)

}

#endif // SWIGXML

//////////////////////////////////////////////////////////
// Maps
//////////////////////////////////////////////////////////

#ifndef SWIGXML


%include std_map.i


// Expose std::map<> as a Python dict
namespace std 
{
#if !defined(SMIL_WRAP_UINT32) && !defined(SMIL_WRAP_UINT)
    %template(Map_UINT) map<UINT,UINT>;
    %template(Map_UINT_double) map<UINT,double>;
    %template(Map_UINT_Vector_double) map<UINT,Vector_double>;
    %template(Map_UINT_Vector_UINT) map<UINT,Vector_UINT>;
#endif

#ifdef USE_64BIT_IDS
    %template(Map_SIZE_T) map<size_t,size_t>;
#endif // USE_64BIT_IDS
    
    TEMPLATE_WRAP_MAP_CROSS_WITH_SECOND_SUBTYPE(vector)
#if !defined(SMIL_WRAP_UINT32) && !defined(SMIL_WRAP_UINT)
    TEMPLATE_WRAP_MAP_CROSS_WITH_SECOND_SUBTYPE_FIX_FIRST(vector, UINT)
#endif

    TEMPLATE_WRAP_CLASS_2T_CROSS(map, Map)
    
#if !defined(SMIL_WRAP_UINT32) && !defined(SMIL_WRAP_UINT)
    TEMPLATE_WRAP_CLASS_2T_FIX_FIRST(map, UINT, Map)
    TEMPLATE_WRAP_CLASS_2T_FIX_SECOND(map, UINT, Map)
#endif

#ifdef USE_64BIT_IDS
    TEMPLATE_WRAP_CLASS_2T_FIX_FIRST(map, size_t, Map)
    TEMPLATE_WRAP_CLASS_2T_FIX_SECOND(map, Vector_size_t, Map)
#endif // USE_64BIT_IDS

#ifndef SMIL_WRAP_double
    TEMPLATE_WRAP_CLASS_2T_FIX_SECOND(map, double, Map)
#endif
    
#ifndef SMIL_WRAP_RGB
    TEMPLATE_WRAP_CLASS_2T_FIX_SECOND(map, RGB, Map)
#endif // SMIL_WRAP_RGB
    TEMPLATE_WRAP_CLASS_2T_FIX_SECOND(map, Vector_double, Map)
#if !defined(SMIL_WRAP_UINT32) && !defined(SMIL_WRAP_UINT)
    TEMPLATE_WRAP_CLASS_2T_FIX_SECOND(map, Vector_UINT, Map)
#endif
    TEMPLATE_WRAP_CLASS_2T_FIX_SECOND(map, Box, Map)
}

#endif // SWIGXML

