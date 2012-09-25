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


#ifndef _DBENCH_H
#define _DBENCH_H

#define FIRST_VA_ARG(arg0, ...) arg0

#define BENCH(func, ...) \
{ \
      int t1 = clock(); \
      for (int i=0;i<BENCH_NRUNS;i++) \
	func(__VA_ARGS__); \
      int t2 = clock(); \
      cout << #func << "\t" << displayTime(double(t2-t1)/BENCH_NRUNS) << endl; \
}

#define BENCH_STR(func, str, ...) \
{ \
      int t1 = clock(); \
      for (int i=0;i<BENCH_NRUNS;i++) \
	func(__VA_ARGS__); \
      int t2 = clock(); \
      cout << #func << " " << str << "\t" << displayTime(double(t2-t1)/BENCH_NRUNS) << endl; \
}

#define BENCH_IMG(func, ...) \
{ \
      int t1 = clock(); \
      for (int i=0;i<BENCH_NRUNS;i++) \
	func(__VA_ARGS__); \
      int t2 = clock(); \
      cout << #func << "\t" << FIRST_VA_ARG(__VA_ARGS__).getTypeAsString() << "\t"; \
      cout << FIRST_VA_ARG(__VA_ARGS__).getWidth() << "x" << FIRST_VA_ARG(__VA_ARGS__).getHeight(); \
      if (FIRST_VA_ARG(__VA_ARGS__).getDepth()>1) cout << "x" << FIRST_VA_ARG(__VA_ARGS__).getDepth(); \
      cout << "\t" << displayTime(double(t2-t1)/BENCH_NRUNS) << endl; \
}

#define BENCH_IMG_STR(func, str, ...) \
{ \
      int t1 = clock(); \
      for (int i=0;i<BENCH_NRUNS;i++) \
	func(__VA_ARGS__); \
      int t2 = clock(); \
      cout << #func << " " << str << "\t" << FIRST_VA_ARG(__VA_ARGS__).getTypeAsString() << "\t"; \
      cout << FIRST_VA_ARG(__VA_ARGS__).getWidth() << "x" << FIRST_VA_ARG(__VA_ARGS__).getHeight(); \
      if (FIRST_VA_ARG(__VA_ARGS__).getDepth()>1) cout << "x" << FIRST_VA_ARG(__VA_ARGS__).getDepth(); \
      cout << "\t" << displayTime(double(t2-t1)/BENCH_NRUNS) << endl; \
}



#endif // _DBENCH_H

