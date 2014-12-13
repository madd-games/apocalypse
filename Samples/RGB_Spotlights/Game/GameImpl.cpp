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

#include <Game/GameImpl.h>
#include <Game/Entity/EntityWall.h>
#include <Apoc/Entity/World.h>
#include <Apoc/Light/SpotLight.h>
#include <Apoc/Light/PointLight.h>
#include <Apoc/Light/DirLight.h>

extern RenderHandler *apocRenderHandler;

void GameImpl::onGameStart()
{
	World::addEntity(new EntityWall());
	apocRenderHandler->setAmbientLight(Vector(0.1, 0.1, 0.1));

	float angle = 90.0*3.14/180.0;
	//SpotLight *red = new SpotLight(Vector(0.0, 0.0, 0.0), Vector(1.0, 1.0, 1.0), Vector(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0, 0.0), angle);
	//PointLight *red = new PointLight(Vector(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0), Vector(0.0, 0.0, 0.0));
	new DirLight(Vector(0.0, 0.0, 1.0), Vector(1.0, 1.0, 1.0), Vector(0.0, 0.0, 0.0));
};
