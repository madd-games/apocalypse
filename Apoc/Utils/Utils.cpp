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

#include <Apoc/Utils/Utils.h>
#include <Apoc/Video/OpenGL.h>
#include <stdlib.h>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

extern float apocRenderTime;		// Main.cpp
extern int   apocDeltaTime;

void ApocFailImpl(string filename, int line, string msg)
{
	SDL_Quit();
	stringstream ss;
	ss << filename << ":" << line << ": " << msg;
#ifdef _WIN32
	MessageBox(NULL, ss.str().c_str(), "ApocFail", MB_OK | MB_ICONHAND);
#else
	cerr << ss.str() << endl;
#endif
	exit(1);
};

float ApocGetRenderTime()
{
	return apocRenderTime;
};

float ApocGetFPS()
{
	return 1.0/apocRenderTime;
};

int ApocGetDeltaTime()
{
	if (apocDeltaTime == 0) return 1;
	return apocDeltaTime;
};
