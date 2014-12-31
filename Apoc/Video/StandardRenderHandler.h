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

#ifndef APOC_VIDEO_STANDARD_RENDER_HANDLER_H
#define APOC_VIDEO_STANDARD_RENDER_HANDLER_H

#include <Apoc/Video/RenderHandler.h>
#include <Apoc/Math/Matrix.h>

/**
 * \brief The standard render handler.
 */
class StandardRenderHandler : public RenderHandler
{
private:
	Vector ambient;

	GLuint shadowFramebuffer;
	GLuint shadowTex;

	GLuint renderProgram;
	ShaderArray<RenderHandler::PointLight> *pointLightArray;
	ShaderArray<RenderHandler::DirLight> *dirLightArray;
	ShaderArray<RenderHandler::SpotLight> *spotLightArray;
	Matrix lightMatrix;

	// default textures
	GLuint defImageTex;
	GLuint defSpecularMap;
	GLuint defNormalMap;
	GLuint defIllumMap;
	GLuint defWarpMap;

	int screenWidth, screenHeight;

	// fog
	Vector fogColor;
	float fogDensity;

	int debugMode;

public:
	/**
	 * \brief Standard render params.
	 */
	struct RenderParams
	{
		
	};

	StandardRenderHandler(int screenWidth, int screenHeight);
	virtual void render();
	virtual void getAttrLocations(GLint &attrVertex, GLint &attrTexCoords, GLint &attrNormal);
	virtual GLint getUniformLocation(const char *name);
	virtual GLint getAttrLocation(const char *name);
	virtual void bindProgram();
	virtual ShaderArray<PointLight> *getPointLightArray();
	virtual ShaderArray<DirLight> *getDirLightArray();
	virtual ShaderArray<SpotLight> *getSpotLightArray();
	virtual void bindDefaultTextures();
	virtual void setAmbientLight(Vector ambient);
	virtual void setFog(Vector color, float density);
	virtual void setDebugMode(int mode);
	virtual void setAttenuation(Vector att);
};

#endif
