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

#include <Apoc/Light/SpotLight.h>
#include <math.h>

extern RenderHandler *apocRenderHandler;

SpotLight::SpotLight(Vector pos, Vector diffuse, Vector specular, Vector axis, float angle)
{
	spotLightArray = apocRenderHandler->getSpotLightArray();
	data.pos = pos;
	data.diffuse = diffuse;
	data.specular = specular;
	axis = axis.normalize();
	data.axisAndCosAngle = Vector(axis[0], axis[1], axis[2], cos(angle));
	key = spotLightArray->add(data);
};

SpotLight::~SpotLight()
{
	spotLightArray->remove(key);
};

void SpotLight::setPosition(Vector pos)
{
	data.pos = pos;
	spotLightArray->set(key, data);
};

Vector SpotLight::getPosition()
{
	return data.pos;
};

void SpotLight::setDiffuse(Vector diffuse)
{
	data.diffuse = diffuse;
	spotLightArray->set(key, data);
};

Vector SpotLight::getDiffuse()
{
	return data.diffuse;
};

void SpotLight::setSpecular(Vector specular)
{
	data.specular = specular;
	spotLightArray->set(key, data);
};

Vector SpotLight::getSpecular()
{
	return data.specular;
};

void SpotLight::setAxis(Vector axis)
{
	axis = axis.normalize();
	data.axisAndCosAngle[0] = axis[0];
	data.axisAndCosAngle[1] = axis[1];
	data.axisAndCosAngle[2] = axis[2];
	spotLightArray->set(key, data);
};

Vector SpotLight::getAxis()
{
	return Vector(data.axisAndCosAngle[0], data.axisAndCosAngle[1], data.axisAndCosAngle[2], 0);
};

void SpotLight::setAngle(float angle)
{
	data.axisAndCosAngle[3] = cos(angle);
};

float SpotLight::getAngle()
{
	return acos(data.axisAndCosAngle[3]);
};

void SpotLight::transform(Matrix mat)
{
	setPosition(mat * getPosition());
	setAxis(mat * getAxis());
};
