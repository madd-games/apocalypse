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

#include <Apoc/Entity/EntityMobile.h>
#include <SDL.h>
#include <math.h>

EntityMobile::EntityMobile(Model::ObjDef *defs, Vector eye, Vector ref)
	: Entity(defs), eyePos(eye), eyeRef(ref), theta(0.0), phi(0.0), deltaFactor(0.0),
	  forward(false), backwards(false), left(false), right(false)
{
	ticks = SDL_GetTicks();
};

Vector EntityMobile::getEyeDelta()
{
	return Vector(0, 0.5 * (sin(deltaFactor) + 1), 0, 0);
};

Vector EntityMobile::getEye()
{
	return getModelMatrix() * eyePos + getEyeDelta();
};

Vector EntityMobile::getRef()
{
	return getEye() + Vector(sin(theta)*cos(phi), sin(phi), cos(theta));
};

Vector EntityMobile::getUpVector()
{
	//return getModelMatrix() * upVector;
	return Vector(0, 1, 0, 0);
};

void EntityMobile::moveCamera(float deltaTheta, float deltaPhi)
{
	rotate(0, deltaTheta, 0);
	rotate(0, -deltaTheta, 0, "ApocColl");

	float pi = 4 * atan(1);
	phi += deltaPhi;
	theta += deltaTheta;
	if (phi < -(pi/2.0))
	{
		phi = -(pi/2.0);
	};
	if (phi > (pi/2.0))
	{
		phi = pi/2.0;
	};
};

void EntityMobile::update()
{
	float speed = 0.015 * (SDL_GetTicks() - ticks);
	if (forward || backwards || left || right)
	{
		deltaFactor += 0.007 * (SDL_GetTicks() - ticks);
	};

	Vector vmove(0, 0, 0, 0);
	if (forward)
	{
		vmove = vmove + Vector(speed*sin(theta), 0, speed*cos(theta), 0);
	};
	if (backwards)
	{
		vmove = vmove + Vector(-speed*sin(theta), 0, -speed*cos(theta), 0);
	};
	if (left)
	{
		Vector forward = Vector(sin(theta), 0, cos(theta), 0);
		Vector left = Vector(0, 1, 0, 0).cross(forward);
		vmove = vmove + left * speed;
	};
	if (right)
	{
		Vector forward = Vector(sin(theta), 0, cos(theta), 0);
		Vector left = Vector(0, 1, 0, 0).cross(forward);
		vmove = vmove + left * -speed;
	};

	float sx = 1;
	if (vmove.x() < 0)
	{
		sx = -1;
	};
	float sz = 1;
	if (vmove.z() < 0)
	{
		sz = -1;
	};

	if (move(Vector(vmove.x()+sx, 0, 0)) == NULL)
	{
		translate(Vector(-sx, 0, 0));
	};
	if (move(Vector(0, 0, vmove.z()+sz)) == NULL)
	{
		translate(Vector(0, 0, -sz));
	};

	ticks = SDL_GetTicks();
};
