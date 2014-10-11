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

#ifndef APOC_COMPUTE_KERNEL_H
#define APOC_COMPUTE_KERNEL_H

#ifdef ENABLE_OPENCL

#include <Apoc/Compute/Compute.h>
#include <stdlib.h>

/**
 * \brief A class that describes commands executable on the GPU.
 *
 * Only available if OpenCL is enabled both at compile-time and run-time.
 */
class Kernel
{
private:
	cl_kernel p_kernel;
	size_t roundUp(size_t a, size_t b);

public:
	/**
	 * \brief Constructor.
	 * \param name The name of the requested kernel function from the kernel directory of Apocalypse or the game.
	 */
	Kernel(const char *name);
	~Kernel();

	/**
	 * \brief Sets a kernel argument, see clSetKernelArg().
	 * \param index The argument index.
	 * \param size Size of the argument.
	 * \param value Pointer to the value for the argument.
	 */
	void setArg(unsigned int index, size_t size, void *value);

	/**
	 * \brief Execute the kernel.
	 * \param numThreads The number of threads.
	 *
	 * Please note that more than numThreads may actually execute! You should therefore pass numThreads as an
	 * argument to the kernel.
	 */
	void execute(size_t numThreads);
};

#endif
#endif
