/* Copyright 2015-2017 Philippe Tillet
* 
* Permission is hereby granted, free of charge, to any person obtaining 
* a copy of this software and associated documentation files 
* (the "Software"), to deal in the Software without restriction, 
* including without limitation the rights to use, copy, modify, merge, 
* publish, distribute, sublicense, and/or sell copies of the Software, 
* and to permit persons to whom the Software is furnished to do so, 
* subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be 
* included in all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef ISAAC_BACKEND_KEYWORDS_H
#define ISAAC_BACKEND_KEYWORDS_H

#include "isaac/driver/common.h"
#include "isaac/driver/device.h"
namespace isaac
{

class keyword
{
public:
  keyword(driver::backend_type backend, std::string const & opencl, std::string const & cuda);
  std::string const & get() const;
private:
  driver::backend_type backend_;
  std::string opencl_;
  std::string cuda_;
};

static inline std::string size_type(driver::Device const & device)
{
  switch(device.backend())
  {
    case driver::CUDA:
      return "int";
    case driver::OPENCL:
      return "int";
    default:
      throw;
  }
}

std::ostream &  operator<<(std::ostream & ss, keyword const & kw);

#define ADD_KEYWORD(NAME, OCLKW, CUDAKW) class NAME : public keyword { public: NAME(driver::backend_type backend) : keyword(backend, OCLKW, CUDAKW){} };

ADD_KEYWORD(KernelPrefix, "__kernel", "extern \"C\" __global__")
ADD_KEYWORD(Local, "__local", "__shared__")
ADD_KEYWORD(Global, "__global", "")
ADD_KEYWORD(LocalPtr, "__local", "")

ADD_KEYWORD(GlobalIdx0, "get_global_id(0)", "(blockIdx.x*blockDim.x + threadIdx.x)")
ADD_KEYWORD(GlobalIdx1, "get_global_id(1)", "(blockIdx.y*blockDim.y + threadIdx.y)")
ADD_KEYWORD(GlobalIdx2, "get_global_id(2)", "(blockIdx.z*blockDim.z + threadIdx.z)")

ADD_KEYWORD(GlobalSize0, "get_global_size(0)", "(blockDim.x*gridDim.x)")
ADD_KEYWORD(GlobalSize1, "get_global_size(1)", "(blockDim.y*gridDim.y)")
ADD_KEYWORD(GlobalSize2, "get_global_size(2)", "(blockDim.z*gridDim.z)")

ADD_KEYWORD(LocalIdx0, "get_local_id(0)", "threadIdx.x")
ADD_KEYWORD(LocalIdx1, "get_local_id(1)", "threadIdx.y")
ADD_KEYWORD(LocalIdx2, "get_local_id(2)", "threadIdx.z")

ADD_KEYWORD(LocalSize0, "get_local_size(0)", "blockDim.x")
ADD_KEYWORD(LocalSize1, "get_local_size(1)", "blockDim.y")
ADD_KEYWORD(LocalSize2, "get_local_size(2)", "blockDim.z")

ADD_KEYWORD(GroupIdx0, "get_group_id(0)", "blockIdx.x")
ADD_KEYWORD(GroupIdx1, "get_group_id(1)", "blockIdx.y")
ADD_KEYWORD(GroupIdx2, "get_group_id(2)", "blockIdx.z")

ADD_KEYWORD(GroupSize0, "get_ng(0)", "GridDim.x")
ADD_KEYWORD(GroupSize1, "get_ng(1)", "GridDim.y")
ADD_KEYWORD(GroupSize2, "get_ng(2)", "GridDim.z")

ADD_KEYWORD(LocalBarrier, "barrier(CLK_LOCAL_MEM_FENCE)", "__syncthreads()")
struct CastPrefix: public keyword{ CastPrefix(driver::backend_type backend, std::string const & datatype): keyword(backend, "convert_" + datatype, "make_" + datatype){} };
struct InitPrefix: public keyword{ InitPrefix(driver::backend_type backend, std::string const & datatype): keyword(backend, "", "make_" + datatype){} };

struct Infinity: public keyword{ Infinity(driver::backend_type backend, std::string const & datatype): keyword(backend, "INFINITY", "infinity<" + datatype + ">()"){} };
struct Select: public keyword{ Select(driver::backend_type backend, std::string cond, std::string if_value, std::string else_value): keyword(backend, "select(" + else_value + "," + if_value + "," + cond + ")", "(" + cond + ")?" + if_value + ":" + else_value) {} };
#undef ADD_KEYWORD


}
#endif
