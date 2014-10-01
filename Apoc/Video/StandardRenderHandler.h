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

#ifndef APOC_VIDEO_STANDARD_RENDER_HANDLER_H
#define APOC_VIDEO_STANDARD_RENDER_HANDLER_H

#include <Apoc/Video/RenderHandler.h>

/**
 * \brief The standard render handler.
 * 
 * Renders the world without any special effects. This is used by default if no RenderHandler
 * is defined by the game.
 */
class StandardRenderHandler : public RenderHandler
{
private:
	GLuint renderProgram;
	GLuint dirLightTex;
	int numDirLights;

public:
	/**
	 * \brief Standard render params.
	 */
	struct RenderParams
	{
		
	};

	StandardRenderHandler();
	virtual void render();
	virtual void getAttrLocations(GLint &attrVertex, GLint &attrTexCoords, GLint &attrNormal);
	virtual GLint getUniformLocation(const char *name);
	virtual void bindProgram();
	virtual void setDirLights(DirLight *array, int count);
};

#endif
