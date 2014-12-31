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

#ifndef APOC_ENTITY_TEXTURE_H
#define APOC_ENTITY_TEXTURE_H

#include <Apoc/Video/OpenGL.h>
#include <map>
#include <string>
#include <inttypes.h>

using namespace std;

/**
 * \brief A class that represents textures on the GPU.
 */
class Texture
{
private:
	static map<string, Texture*> texMap;
	GLuint texObj;
	void doLoad(const int width, const int height, void *data, bool allowMipmaps);

public:
	/**
	 * \brief Initialise the texturing system by loading the texture map.
	 */
	static void Init();

	/**
	 * \brief Returns a texture given its name.
	 */
	static Texture* Get(string name);

	/**
	 * \brief Describes a texel.
	 */
	struct Texel
	{
		uint8_t red, green, blue, alpha;
	};

	/**
	 * \brief Used to describe textures in the binary.
	 */
	struct Map
	{
		const char *name;
		int width;
		int height;
		const char *filename;
		bool allowMipmaps;
	};

	/**
	 * \brief Constructor.
	 * \param width The width of the texture.
	 * \param height The height of the texture.
	 * \param filename Name of the texture file (in the path).
	 * \param allowMipmaps Whether to allow mipmaps or not.
	 */
	Texture(const int width, const int height, string filename, bool allowMipmaps = true);

	/**
	 * \brief Constructor.
	 * \param width Width of the texture.
	 * \param height Height of the texture.
	 * \param data The pixel data.
	 * \param allowMipmaps Whether to allow mipmaps or not.
	 */
	Texture(const int width, const int height, void *data, bool allowMipmaps = true);

	/**
	 * \brief Destructor.
	 *
	 * Deletes the OpenGL texture object.
	 */
	~Texture();

	/**
	 * \brief Bind the texture to the current active texture unit.
	 */
	void bind();
};

#endif
