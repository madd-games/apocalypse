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

#include <Apoc/Entity/Entity.h>
#include <Apoc/Utils/Utils.h>
#include <inttypes.h>
#include <Apoc/Video/RenderHandler.h>

extern RenderHandler *apocRenderHandler;

Entity::Entity(Model::ObjDef *defs)
{
	modelMatrix = Matrix::Identity();

	while (defs->vertices != NULL)
	{
		Object obj;
		if (defs->model == NULL)
		{
			obj.model = new Model(defs->vertices, defs->count);
			defs->model = obj.model;
		}
		else
		{
			obj.model = defs->model;
		};

		string texName = defs->texName;
		obj.textures[0] = Texture::Get(texName);
		obj.matrix = Matrix::Identity();
		obj.diffuseColor = defs->diffuseColor;
		obj.specularColor = defs->specularColor;
		objects[defs->name] = obj;
		defs++;
	};
};

Entity::~Entity()
{
};

void Entity::transform(string obj, Matrix mat)
{
	if (obj == "")
	{
		modelMatrix = mat * modelMatrix;
	}
	else
	{
		objects[obj].matrix = mat * objects[obj].matrix;
	};
};

void Entity::preTransform(string obj, Matrix mat)
{
	if (obj == "")
	{
		modelMatrix = modelMatrix * mat;
	}
	else
	{
		objects[obj].matrix = objects[obj].matrix * mat;
	};
};

void Entity::update()
{
};

void Entity::renderObjects()
{
	GLint uModelMatrix = apocRenderHandler->getUniformLocation("uModelMatrix");
	GLint uObjectMatrix = apocRenderHandler->getUniformLocation("uObjectMatrix");
	GLint uDiffuseColor = apocRenderHandler->getUniformLocation("uDiffuseColor");
	GLint uSpecularColor = apocRenderHandler->getUniformLocation("uSpecularColor");

	glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, &modelMatrix[0][0]);

	map<string, Object>::iterator it;
	for (it=objects.begin(); it!=objects.end(); ++it)
	{
		map<unsigned int, Texture*>::iterator jt;
		for (jt=it->second.textures.begin(); jt!=it->second.textures.end(); ++jt)
		{
			glActiveTexture(GL_TEXTURE0 + jt->first);
			jt->second->bind();
		};

		glUniform4fv(uDiffuseColor, 1, &it->second.diffuseColor[0]);
		glUniform4fv(uSpecularColor, 1, &it->second.specularColor[0]);
		glUniformMatrix4fv(uObjectMatrix, 1, GL_FALSE, &it->second.matrix[0][0]);
		it->second.model->draw();
	};
};

void Entity::translate(Vector vec)
{
	transform("", Matrix::Translate(-vec.x(), vec.y(), vec.z()));
};

void Entity::rotate(float x, float y, float z)
{
	preTransform("", Matrix::Rotate(x, y, z));
};
