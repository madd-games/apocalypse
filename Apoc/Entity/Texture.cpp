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

#include <Apoc/Entity/Texture.h>
#include <Apoc/Video/OpenGL.h>
#include <Apoc/Utils/Utils.h>

map<string, Texture*> Texture::texMap;

extern Texture::Map apocTextureMap[];

void Texture::Init()
{
	Texture::Map *iter = apocTextureMap;
	while (iter->name != NULL)
	{
		string name(iter->name);
		texMap[name] = new Texture(iter->width, iter->height, iter->data);
		iter++;
	};
};

Texture* Texture::Get(string name)
{
	if (texMap.count(name) == 0)
	{
		ApocFail("No such texture: " + name);
	};
	return texMap[name];
};

Texture::Texture(const int width, const int height, const Texture::Texel *data)
{
	glGenTextures(1, &texObj);
	glBindTexture(GL_TEXTURE_2D, texObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
};

Texture::~Texture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &texObj);
};

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, texObj);
};
