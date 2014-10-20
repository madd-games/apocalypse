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
#include <Apoc/Utils/Utils.h>

// Texture units:
// 0 = main (diffuse) texture
// 1 = directional light array
// 2 = point light array
// 3 = shadow map
// 4 = specular map
// 5 = normal map

extern "C" const char *stdVertexShader;
extern "C" const char *stdFragmentShader;
unsigned long tmpTimer;
float lmatheight = -0.3;

const float defImageTexData[] = {
	0.3, 0.3, 1.0, 1.0,		1.0, 1.0, 1.0, 1.0,
	1.0, 1.0, 1.0, 1.0,		0.3, 0.3, 1.0, 1.0
};

const float defSpecularMapData[] = {
	1.0, 1.0, 1.0, 1.0
};

const float defNormalMapData[] = {
	0.0, 0.0, 1.0, 1.0
};

StandardRenderHandler::StandardRenderHandler(int screenWidth, int screenHeight)
	: numDirLights(0), numPointLights(0), screenWidth(screenWidth), screenHeight(screenHeight)
{
	tmpTimer = SDL_GetTicks();
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

	// default image texture.
	glGenTextures(1, &defImageTex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, defImageTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, defImageTexData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// default specular map.
	glGenTextures(1, &defSpecularMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, defSpecularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, defSpecularMapData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// default normal map.
	glGenTextures(1, &defNormalMap);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, defNormalMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, defNormalMapData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// the shadowmap framebuffer
	glGenFramebuffers(1, &shadowFramebuffer);
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1366, 768, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		ApocFail("The shadowmap framebuffer is incomplete!");
	};

	Matrix view = Matrix::LookAt(
		Vector(0.0, -0.1, -1.0),
		Vector(0.0, 1.0, 0.0),
		Vector(0.0, 0.0, 0.0)
	);
	lightMatrix = Matrix::Ortho(20, -20, 20, -20, -30, 20) * view;
	//cout << "Light matrix: " << endl << lightMatrix << endl;

	//Vector point(-11.000000, 0.000000, 10.999980, 1);
	//cout << "Test point: " << (lightMatrix * point) << endl;
};

void StandardRenderHandler::render()
{
	if ((SDL_GetTicks()-tmpTimer) > 100)
	{
		//lmatheight -= 0.01;
		tmpTimer = SDL_GetTicks();
	};

	Matrix view = Matrix::LookAt(
		Vector(0.0, lmatheight, -1.0),
		Vector(0.0, 1.0, 0.0),
		Vector(0.0, 0.0, 0.0)
	);
	lightMatrix = Matrix::Ortho(20, -20, 20, -20, -20, 10) * view;

	//glDepthFunc(GL_LESS);
	glUniform1i(getUniformLocation("uIsParticle"), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);
	glViewport(0, 0, 1366, 768);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUniform1i(getUniformLocation("uIsShadowMap"), 1);
	glDrawBuffer(GL_NONE);
	Matrix identity = Matrix::Identity();
	glUniformMatrix4fv(getUniformLocation("uProjectionMatrix"), 1, GL_FALSE, &identity[0][0]);
	glUniformMatrix4fv(getUniformLocation("uViewMatrix"), 1, GL_FALSE, &lightMatrix[0][0]);
	World::render(false);
	//return;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screenWidth, screenHeight);
	//glUseProgram(renderProgram);
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniformMatrix4fv(getUniformLocation("uLightMatrix"), 1, GL_FALSE, &lightMatrix[0][0]);
	glUniform1i(getUniformLocation("uIsShadowMap"), 0);
	glUniform4f(getUniformLocation("uAmbientLight"), 0.1, 0.1, 0.1, 1.0);
	glUniform1i(getUniformLocation("uDirLightArray"), 1);
	glUniform1i(getUniformLocation("uPointLightArray"), 2);
	glUniform1i(getUniformLocation("uShadowMap"), 3);
	glUniform1i(getUniformLocation("uSpecularMap"), 4);
	glUniform1i(getUniformLocation("uNormalMap"), 5);
	glUniform1i(getUniformLocation("uNumDirLights"), numDirLights);
	glUniform1i(getUniformLocation("uNumPointLights"), numPointLights);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glEnable(GL_CULL_FACE);
	World::render();

	glUniformMatrix4fv(getUniformLocation("uModelMatrix"), 1, GL_FALSE, &identity[0][0]);
	glUniformMatrix4fv(getUniformLocation("uObjectMatrix"), 1, GL_FALSE, &identity[0][0]);
	glUniform1i(getUniformLocation("uIsParticle"), 1);
	World::renderParticles();
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

GLint StandardRenderHandler::getAttrLocation(const char *name)
{
	return glGetAttribLocation(renderProgram, name);
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

void StandardRenderHandler::bindDefaultTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, defImageTex);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, defSpecularMap);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, defNormalMap);
};
