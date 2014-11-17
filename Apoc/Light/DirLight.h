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

#ifndef APOC_DIR_LIGHT_H
#define APOC_DIR_LIGHT_H

#include <Apoc/Light/Light.h>
#include <Apoc/Video/RenderHandler.h>

/**
 * \brief Directional light.
 *
 * A directional light glows in a specified direction from everywhere at once, with all light rays being
 * parallel.
 */
class DirLight : public Light
{
private:
	ShaderArray<RenderHandler::DirLight> *dirLightArray;
	RenderHandler::DirLight data;
	int key;

public:
	/**
	 * \brief Constructor.
	 * \param dir Direction in which the last is cast.
	 * \param diffuse Intensity of the diffuse light source (for each component).
	 * \param specular Intensity of the specular light source (for each component).
	 */
	DirLight(Vector dir, Vector diffuse, Vector specular);

	virtual ~DirLight();

	/**
	 * \brief Set the direction of the light source.
	 */
	void setDirection(Vector dir);

	/**
	 * \brief Get the direction of the light source.
	 */
	Vector getDirection();

	/**
	 * \brief Set the diffuse itensity of the light source.
	 */
	void setDiffuse(Vector diffuse);

	/**
	 * \brief Get the diffuse intensity of the light source.
	 */
	Vector getDiffuse();

	/**
	 * \brief Set the specular intensity of the light source.
	 */
	void setSpecular(Vector specular);

	/**
	 * \brief Get the specular intensity of the light source.
	 */
	Vector getSpecular();
};

#endif
