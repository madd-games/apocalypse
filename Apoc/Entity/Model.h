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

#ifndef APOC_ENTITY_MODEL_H
#define APOC_ENTITY_MODEL_H

#include <Apoc/Math/Vector.h>
#include <Apoc/Video/RenderHandler.h>
#include <Apoc/Entity/Texture.h>
#include <map>

extern RenderHandler *apocRenderHandler;

using namespace std;

/**
 * \brief A class for storing models.
 */
class Model
{
private:
	GLuint vao, vbo;
	int vertexCount;

public:
	/**
	 * \brief A structure that represents a vertex.
	 */
	struct Vertex
	{
		Vector pos;
		Vector texCoords;
		Vector normal;
	};

	/**
	 * \brief An object definition.
	 */
	struct ObjDef
	{
		const char *name;
		Vertex *vertices;
		int count;
		Model *model;		// initially NULL
		const char *texName;
		Vector diffuseColor;
		Vector specularColor;
		float shininess;	// ie. specular exponent
		const char *specTex;	// specular texture name.
	};

	/**
	 * \brief Constructor.
	 * 
	 * Given a list of vertices and the size, this will create the VAO and VBO to store
	 * the model on the GPU. The vertices array may be deleted after the Model object is
	 * constructed.
	 */
	Model(Vertex *vertices, const int count);

	/**
	 * \brief Destructor.
	 *
	 * Deletes the VAO and VBO.
	 */
	~Model();

	/**
	 * \brief Draw the model.
	 *
	 * This function will call glDrawArrays() with the VAO and VBO bound, but without modifying the
	 * shader uniforms. It is up to the RenderHandler to set the uniforms as appropriate.
	 */
	void draw();
};

#endif
