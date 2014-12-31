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

#include <Apoc/Entity/Texture.h>
#include <Apoc/Video/OpenGL.h>
#include <Apoc/Utils/Utils.h>
#include <Apoc/Utils/Archive.h>

map<string, Texture*> Texture::texMap;

extern Texture::Map apocTextureMap[];

void Texture::Init()
{
	Texture::Map *iter = apocTextureMap;
	while (iter->name != NULL)
	{
		string name(iter->name);
		texMap[name] = new Texture(iter->width, iter->height, iter->filename, iter->allowMipmaps);
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

void Texture::doLoad(const int width, const int height, void *data, bool allowMipmaps)
{
	glEnable(GL_TEXTURE_2D);		// work around AMD bugs, apparently.
						// (idk, i have nVidia).
	glGenTextures(1, &texObj);
	glBindTexture(GL_TEXTURE_2D, texObj);
	glTexStorage2D(GL_TEXTURE_2D, allowMipmaps ? 8:1, GL_RGBA8, width, height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
};

Texture::Texture(const int width, const int height, string filename, bool allowMipmaps)
{
	DataFile file(filename);
	uint8_t *data = new uint8_t[width * height * 4];
	file.read(data, width * height * 4);
	doLoad(width, height, data, allowMipmaps);
	delete data;
};

Texture::Texture(const int width, const int height, void *data, bool allowMipmaps)
{
	doLoad(width, height, data, allowMipmaps);
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
