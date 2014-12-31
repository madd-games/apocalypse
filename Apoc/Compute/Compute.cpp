/*
	Copyright (c) 2014-2015, Madd Games.
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

// This file is only compiled when OpenCL is enabled for Apolcalypse.
#ifdef ENABLE_OPENCL

#include <Apoc/Compute/Compute.h>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <GL/glx.h>
#endif

using namespace std;

cl_device_id p_device;
cl_context p_context;
cl_command_queue p_command_queue;
cl_program p_program;
bool apocComputeEnabled = false;

extern const char *kernelCode;

void InitCompute()
{
	cout << "[APOC] [COMPUTE] Attempting to create a shared CL/GL context" << endl;

	cl_int error = 0;
	cl_platform_id platform;

	error = clGetPlatformIDs(1, &platform, NULL);
	if (error != CL_SUCCESS)
	{
		cerr << "[APOC] [COMPUTE] Error: could not get platform ID; falling back to software kernels." << endl;
		return;
	};

	error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &p_device, NULL);
	if (error != CL_SUCCESS)
	{
		cerr << "[APOC] [COMPUTE] Error: could not get the GPU device ID; falling back to software kernels." << endl;
		return;
	};

#ifdef _WIN32
	cout << "[APOC] [COMPUTE] Not implemented under Windows yet; falling back to software kernels." << endl;
	cl_context_properites props[];
	return;
#else
	cl_context_properties props[] =
	{
		CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
		CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
		CL_CONTEXT_PLATFORM, (cl_context_properties)(platform),
		0
	};
#endif

	p_context = clCreateContextFromType(props, CL_DEVICE_TYPE_GPU, NULL, NULL, &error);
	if (error != CL_SUCCESS)
	{
		cerr << "[APOC] [COMPUTE] Error: could not create CL/GL context; falling back to software kernels." << endl;
		return;
	};

	error = CL_SUCCESS;
	p_command_queue = clCreateCommandQueue(p_context, p_device, 0, &error);
	if (error != CL_SUCCESS)
	{
		cerr << "[APOC] [COMPUTE] Error: could not create the command queue; falling back to software kernels." << endl;
		return;
	};

	cl_program program = clCreateProgramWithSource(p_context, 1, &kernelCode, NULL, &error);
	if (error != CL_SUCCESS)
	{
		cerr << "[APOC] [COMPUTE] Error: could not create the program object; falling back to software kernels." << endl;
		return;
	};

	error = clBuildProgram(program, 1, &p_device, NULL, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		cerr << "[APOC] [COMPUTE] Errors occured while building the OpenCL program." << endl;

		// Shows the log
		cout << "BUILD LOG:" << endl;
		char* build_log;
		size_t log_size;
		// First call to know the proper size
		clGetProgramBuildInfo(program, p_device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		build_log = new char[log_size+1];
		// Second call to get the log
		clGetProgramBuildInfo(program, p_device, CL_PROGRAM_BUILD_LOG, log_size, build_log, NULL);
		build_log[log_size] = '\0';
		cout << build_log << endl;
		delete[] build_log;
		cout << "[APOC] [COMPUTE] Falling back to software kernels!" << endl;
		return;
	};

	cout << "[APOC] [COMPUTE] OpenCL initialisation successful; enabling CL kernels." << endl;
	p_program = program;
	apocComputeEnabled = true;
};

void QuitCompute()
{
	clReleaseContext(p_context);
};

bool IsComputeEnabled()
{
	return apocComputeEnabled;
};

void AcquireGL(size_t count, const cl_mem *mem)
{
	clEnqueueAcquireGLObjects(p_command_queue, count, mem, 0, NULL, NULL);
};

void ReleaseGL(size_t count, const cl_mem *mem)
{
	clEnqueueReleaseGLObjects(p_command_queue, count, mem, 0, NULL, NULL);
};

#endif
