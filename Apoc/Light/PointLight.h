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

#ifndef APOC_POINT_LIGHT_H
#define APOC_POINT_LIGHT_H

#include <Apoc/Light/Light.h>
#include <Apoc/Video/RenderHandler.h>

/**
 * \brief Point light.
 *
 * A point light glows from a specified point in space and its intensity is inversely
 * proportional to the distance from the source.
 */
class PointLight : public Light
{
private:
	ShaderArray<RenderHandler::PointLight> *pointLightArray;
	RenderHandler::PointLight data;
	int key;

public:
	/**
	 * \brief Constructor.
	 * \param pos Position of the source.
	 * \param diffuse Intensity of the diffuse light source (for each component).
	 * \param specular Intensity of the specular light source (for each component).
	 */
	PointLight(Vector pos, Vector diffuse, Vector specular);

	virtual ~PointLight();

	/**
	 * \brief Set the position of the light source.
	 */
	void setPosition(Vector pos);

	/**
	 * \brief Get the position of the light source.
	 */
	Vector getPosition();

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

	virtual void transform(Matrix mat);
};

#endif
