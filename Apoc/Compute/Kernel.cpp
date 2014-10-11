/*
	Copyright (c) 2014, Madd Games.
	All rights reserved.
	
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	
	* Redistributions of source code must retain the above copyright notice, this
	  list of conditions and the following disclaimer.
	
	* Redistributions in binary form must reproduce the above copyright notice,
	  this list of conditions and the following disclaimer in the documentation
	  and/or other materials provided with the distribution.
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef ENABLE_OPENCL

#include <Apoc/Compute/Kernel.h>
#include <Apoc/Utils/Utils.h>
#include <sstream>

extern cl_program p_program;
extern cl_command_queue p_command_queue;

using namespace std;

size_t Kernel::roundUp(size_t a, size_t b)
{
	size_t out = a;
	while (out < b)
	{
		out += a;
	};
	return out;
};

Kernel::Kernel(const char *name)
{
	cl_int error;
	p_kernel = clCreateKernel(p_program, name, &error);
	if (error != CL_SUCCESS)
	{
		stringstream ss;
		ss << "OpenCL returned an error while trying to create a kernel: " << name;
		ApocFail(ss.str());
	};
};

Kernel::~Kernel()
{
	clReleaseKernel(p_kernel);
};

void Kernel::setArg(unsigned int index, size_t size, void *value)
{
	if (clSetKernelArg(p_kernel, index, size, value) != CL_SUCCESS)
	{
		ApocFail("Kernel::setArg");
	};
};

void Kernel::execute(size_t numThreads)
{
	size_t local_ws = 512;
	size_t global_ws = roundUp(local_ws, numThreads);
	cl_int error = clEnqueueNDRangeKernel(p_command_queue, p_kernel, 1, NULL, &global_ws, &local_ws, 0, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		cerr << "Error during kernel execution: " << error << endl;
		ApocFail("Kernel::execute");
	};
};

#endif
