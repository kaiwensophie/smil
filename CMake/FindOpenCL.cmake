# Copyright (c) 2011, Matthieu FAESSEL and ARMINES
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of Matthieu FAESSEL, or ARMINES nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# Toolchain configuration for clang/llvm


FIND_PATH(OPENCL_INCLUDE_DIR
	NAMES
		CL/cl.h OpenCL/cl.h
	PATHS
		$ENV{AMDAPPSDKROOT}/include
		$ENV{INTELOCLSDKROOT}/include
		$ENV{NVSDKCOMPUTE_ROOT}/OpenCL/common/inc
		# Legacy Stream SDK
		$ENV{ATISTREAMSDKROOT}/include)

IF(USE_64BIT_IDS)
	SET(OPENCL_LIB_SEARCH_PATH
		${OPENCL_LIB_SEARCH_PATH}
		$ENV{AMDAPPSDKROOT}/lib/x86
		$ENV{INTELOCLSDKROOT}/lib/x86
		$ENV{NVSDKCOMPUTE_ROOT}/OpenCL/common/lib/Win32
		# Legacy Stream SDK
		$ENV{ATISTREAMSDKROOT}/lib/x86)
ELSE(USE_64BIT_IDS)
	SET(OPENCL_LIB_SEARCH_PATH
		${OPENCL_LIB_SEARCH_PATH}
		$ENV{AMDAPPSDKROOT}/lib/x86_64
		$ENV{INTELOCLSDKROOT}/lib/x64
		$ENV{NVSDKCOMPUTE_ROOT}/OpenCL/common/lib/x64
		# Legacy stream SDK
		$ENV{ATISTREAMSDKROOT}/lib/x86_64)
ENDIF(USE_64BIT_IDS)

FIND_LIBRARY(OPENCL_LIBRARY
	NAMES OpenCL
	PATHS ${OPENCL_LIB_SEARCH_PATH})

INCLUDE(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(
		OpenCL
		DEFAULT_MSG
		OPENCL_LIBRARY OPENCL_INCLUDE_DIR)

IF(OPENCL_FOUND)
	SET(OPENCL_LIBRARIES ${OPENCL_LIBRARY})
	MARK_AS_ADVANCED(OPENCL_INCLUDE_DIR OPENCL_LIBRARY)
ELSE(OPENCL_FOUND)
	SET(OPENCL_LIBRARIES)
ENDIF(OPENCL_FOUND)

