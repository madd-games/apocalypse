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

#include <Apoc/Thread/Thread.h>
#include <stdlib.h>

#ifndef _WIN32
#include <signal.h>
#endif

struct ThreadInfo
{
	ThreadHandler *handler;
	Thread *thread;
};

#ifdef _WIN32
DWORD ApocThreadProc(LPVOID pHandler)
#else
void* ApocThreadProc(void *pHandler)
#endif
{
	ThreadInfo *info = reinterpret_cast<ThreadInfo*>(pHandler);
	ThreadHandler *handler = info->handler;
	Thread *thread = info->thread;
	delete info;
	handler->startThread(thread);
	
#ifdef _WIN32
	return 0;
#else
	return NULL;
#endif
};

Thread::Thread(ThreadHandler *handler)
{
	threadHandler = handler;
	
	ThreadInfo *info = new ThreadInfo;
	info->handler = handler;
	info->thread = this;
	
#ifdef _WIN32
	sysThread = CreateThread(NULL, 0, ApocThreadProc, reinterpret_cast<LPVOID>(info), 0, NULL);
#else
	pthread_create(&sysThread, NULL, ApocThreadProc, reinterpret_cast<LPVOID>(info));
#endif
};

Thread::~Thread()
{
#ifdef _WIN32
	TerminateThread(sysThread, 0);
#else
	pthread_kill(sysThread, SIGKILL);
#endif

	join();
	delete threadHandler;
};

void Thread::join()
{
#ifdef _WIN32
	WaitForSingleObject(sysThread, INFINITE);
	CloseHandler(sysThread);
#else
	pthread_join(sysThread, NULL);
#endif
};
