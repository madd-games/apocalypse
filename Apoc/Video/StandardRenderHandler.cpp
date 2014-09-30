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

#include <Apoc/Video/StandardRenderHandler.h>
#include <Apoc/Entity/World.h>

extern "C" const char *stdVertexShader;
extern "C" const char *stdFragmentShader;

StandardRenderHandler::StandardRenderHandler()
{
	renderProgram = createProgram(stdVertexShader, stdFragmentShader);
};

void StandardRenderHandler::render()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	World::render();
};

void StandardRenderHandler::getAttrLocations(GLint &attrVertex, GLint &attrTexCoords, GLint &attrNormal)
{
	attrVertex = glGetAttribLocation(renderProgram, "inVertex");
	attrTexCoords = glGetAttribLocation(renderProgram, "inTexCoords");
	attrNormal = glGetAttribLocation(renderProgram, "inNormal");
};

GLint StandardRenderHandler::getUniformLocation(const char *name)
{
	return glGetUniformLocation(renderProgram, name);
};

void StandardRenderHandler::bindProgram()
{
	glUseProgram(renderProgram);
	glUniform4f(getUniformLocation("uAmbientLight"), 0.1, 0.1, 0.1, 1.0);
	glUniform3f(getUniformLocation("uLightDir[0]"), 0.7, -1.0, 0.9);
	glUniform3f(getUniformLocation("uLightDir[1]"), -0.6, -1.0, -0.9);
	glUniform4f(getUniformLocation("uDiffuseLight[0]"), 1.0, 1.0, 1.0, 1.0);
	glUniform4f(getUniformLocation("uSpecularLight[0]"), 1.0, 1.0, 1.0, 1.0);
	glUniform4f(getUniformLocation("uDiffuseLight[1]"), 0.7, 0.7, 0.7, 1.0);
	glUniform4f(getUniformLocation("uSpecularLight[1]"), 0.5, 0.0, 0.0, 1.0);
};
