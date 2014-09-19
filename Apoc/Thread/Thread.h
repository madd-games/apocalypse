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

#ifndef APOC_THREAD_THREAD_H
#define APOC_THREAD_THREAD_H

#include <Apoc/Thread/ThreadHandler.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

/**
 * \brief A class representing running threads.
 */
class Thread
{
private:
#ifdef _WIN32
	HANDLE sysThread;
#else
	pthread_t sysThread;
#endif
	ThreadHandler *threadHandler;
	
public:
	/**
	 * \brief Constructor, creates a new thread.
	 * \param handler The thread handler - this is automatically deleted when the thread is deleted.
	 */
	Thread(ThreadHandler *handler);
	
	/**
	 * \brief Destructor.
	 * 
	 * The thread is terminated and the threadHandler is deleted.
	 */
	~Thread();
	
	/**
	 * \brief Join the thread.
	 * 
	 * This function waits until the thread terminates.
	 */
	void join();
};

#endif
