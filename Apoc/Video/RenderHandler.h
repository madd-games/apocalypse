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

#ifndef APOC_VIDEO_RENDER_HANDLER_H
#define APOC_VIDEO_RENDER_HANDLER_H

#include <Apoc/Video/OpenGL.h>
#include <Apoc/Math/Vector.h>

#include <string>

using namespace std;

class World;

/**
 * \brief Base class for render handlers.
 * 
 * You can create a custom render handler in the `Game` directory to apply custom effects
 * etc. to the rendered scene.
 */
class RenderHandler
{
private:

protected:
	/**
	 * \brief Creates an OpenGL program.
	 * 
	 * This function can be used by child classes to help create OpenGL programs
	 * more easily. If the compilation or linking of the shaders fails, the engine
	 * terminates with an ApocFail.
	 * 
	 * \param glslVertex The GLSL Vertex Shader code.
	 * \param glslFragment The GLSL Fragment Shader code.
	 * \return The OpenGL program object.
	 */
	GLuint createProgram(string glslVertex, string glslFragment);
	
public:
	/**
	 * \brief An element of the directional light array.
	 */
	struct DirLight
	{
		/**
		 * \brief The direction in which the light is cast.
		 */
		Vector dir;

		/**
		 * \brief The diffuse component of the light.
		 */
		Vector diffuse;

		/**
		 * \brief The specular component of the light.
		 */
		Vector specular;
	};

	/**
	 * \brief An element of the point light array.
	 */
	struct PointLight
	{
		/**
		 * \brief The position of the light source.
		 */
		Vector pos;

		/**
		 * \brief The diffuse component of the light.
		 */
		Vector diffuse;

		/**
		 * \brief The specular component of the light.
		 */
		Vector specular;
	};

	/**
	 * \brief Constructor.
	 */
	RenderHandler();
	virtual ~RenderHandler();
	
	/**
	 * \brief Render the world.
	 * 
	 * This function is called in a loop on the client.
	 */
	virtual void render() = 0;

	/**
	 * \brief Get attribute locations.
	 * \param attrVertex Return the attribute that stores the vertex position (vec4).
	 * \param attrTexCoords Return the attribute that stores the texture coordinates (vec2).
	 * \param attrNormal Return the attribute that stores the normal (vec3).
	 */
	virtual void getAttrLocations(GLint &attrVertex, GLint &attrTexCoords, GLint &attrNormal) = 0;

	/**
	 * \brief Get the location of a uniform variable.
	 * \param name The name of the uniform variable.
	 * \return The location of the uniform variable.
	 */
	virtual GLint getUniformLocation(const char *name) = 0;

	/**
	 * \brief Get the location of an attribute.
	 * \param name The name of the attribute.
	 */
	virtual GLint getAttrLocation(const char *name) = 0;

	virtual void bindProgram() = 0;

	/**
	 * \brief Set the array of directional lights.
	 * \param array The array.
	 * \param count The number of lights.
	 */
	virtual void setDirLights(DirLight *array, int count) = 0;

	/**
	 * \brief Set the array of point lights.
	 * \param array The array.
	 * \param count The number of lights.
	 */
	virtual void setPointLights(PointLight *array, int count) = 0;

	/**
	 * \brief Binds all the defaults textures.
	 */
	virtual void bindDefaultTextures() = 0;

	/**
	 * \brief Sets the amount of ambient light.
	 */
	virtual void setAmbientLight(Vector ambient) = 0;

	/**
	 * \brief Set the color and density of fog.
	 */
	virtual void setFog(Vector color, float density) = 0;
};

#endif
