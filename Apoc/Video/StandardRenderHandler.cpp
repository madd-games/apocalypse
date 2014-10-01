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

// Texture units:
// 0 = main (diffuse) texture
// 1 = directional light array
// 2 = point light array

extern "C" const char *stdVertexShader;
extern "C" const char *stdFragmentShader;

StandardRenderHandler::StandardRenderHandler() : numDirLights(0), numPointLights(0)
{
	renderProgram = createProgram(stdVertexShader, stdFragmentShader);

	// set up the directional light array buffer.
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &dirLightTex);
	glBindTexture(GL_TEXTURE_BUFFER, dirLightTex);
	glGenBuffers(1, &dirLightBuffer);
	glBindBuffer(GL_TEXTURE_BUFFER, dirLightBuffer);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, dirLightBuffer);

	// set up the point light array buffer.
	glActiveTexture(GL_TEXTURE2);
	glGenTextures(1, &pointLightTex);
	glBindTexture(GL_TEXTURE_BUFFER, pointLightTex);
	glGenBuffers(1, &pointLightBuffer);
	glBindBuffer(GL_TEXTURE_BUFFER, pointLightBuffer);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, pointLightBuffer);
};

void StandardRenderHandler::render()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(renderProgram);
	glUniform4f(getUniformLocation("uAmbientLight"), 0.1, 0.1, 0.1, 1.0);
	glUniform1i(getUniformLocation("uDirLightArray"), 1);
	glUniform1i(getUniformLocation("uPointLightArray"), 2);
	glUniform1i(getUniformLocation("uNumDirLights"), numDirLights);
	glUniform1i(getUniformLocation("uNumPointLights"), numPointLights);
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
};

void StandardRenderHandler::setDirLights(RenderHandler::DirLight *array, int count)
{
	glBindBuffer(GL_TEXTURE_BUFFER, dirLightBuffer);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(RenderHandler::DirLight)*count, array, GL_DYNAMIC_COPY);
	numDirLights = count;
};

void StandardRenderHandler::setPointLights(RenderHandler::PointLight *array, int count)
{
	glBindBuffer(GL_TEXTURE_BUFFER, pointLightBuffer);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(RenderHandler::PointLight)*count, array, GL_DYNAMIC_COPY);
	numPointLights = count;
};
