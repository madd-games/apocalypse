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

/*! \file */

#ifndef APOC_UTILS_UTILS_H
#define APOC_UTILS_UTILS_H

#include <string>
#include <iostream>

using namespace std;

/**
 * \brief Reports an engine crash.
 * \param x The error message.
 */
#define ApocFail(x) ApocFailImpl(__FILE__, __LINE__, (x))

/*! \cond */
void ApocFailImpl(string filename, int line, string msg);
/*! \endcond */

/**
 * \brief Change the mouse positition relative to the window.
 *
 * Please note that (0, 0) is the top-left corner of the window.
 */
void ApocMoveMouse(int x, int y);

/**
 * \brief Return the time it took to render the last frame.
 */
float ApocGetRenderTime();

/**
 * \brief Returns the FPS.
 *
 * This is the number of frames that would be rendered in one second assuming that
 * the rendering would be the same speed as the last frame rendered.
 */
float ApocGetFPS();

/**
 * \brief Return the difference in time since the last frame in milliseconds.
 */
int ApocGetDeltaTime();

#endif
