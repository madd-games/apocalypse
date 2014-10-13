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
#include <Apoc/Entity/World.h>
#include <Apoc/Utils/Utils.h>
#include <Apoc/Physics/CollisionCheck.h>
#include <inttypes.h>
#include <Apoc/Video/RenderHandler.h>

extern RenderHandler *apocRenderHandler;

Entity::Entity(Model::ObjDef *defs) : bbDirty(true)
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
		string specMap = defs->specTex;
		if (texName != "empty_texture")
		{
			obj.textures[0] = Texture::Get(texName);
		};
		if (specMap != "<NONE>")
		{
			obj.textures[4] = Texture::Get(specMap);
		};
		obj.matrix = Matrix::Identity();
		obj.diffuseColor = defs->diffuseColor;
		obj.specularColor = defs->specularColor;
		obj.shininess = defs->shininess;
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
	bbDirty = true;
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
	bbDirty = true;
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
	GLint uShininess = apocRenderHandler->getUniformLocation("uShininess");

	glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, &modelMatrix[0][0]);

	map<string, Object>::iterator it;
	for (it=objects.begin(); it!=objects.end(); ++it)
	{
		apocRenderHandler->bindDefaultTextures();
		map<unsigned int, Texture*>::iterator jt;
		for (jt=it->second.textures.begin(); jt!=it->second.textures.end(); ++jt)
		{
			glActiveTexture(GL_TEXTURE0 + jt->first);
			jt->second->bind();
		};

		glUniform4fv(uDiffuseColor, 1, &it->second.diffuseColor[0]);
		glUniform4fv(uSpecularColor, 1, &it->second.specularColor[0]);
		glUniform1f(uShininess, it->second.shininess);
		glUniformMatrix4fv(uObjectMatrix, 1, GL_FALSE, &it->second.matrix[0][0]);
		it->second.model->draw();
	};
};

void Entity::translate(Vector vec)
{
	transform("", Matrix::Translate(vec.x(), vec.y(), vec.z()));
};

void Entity::rotate(float x, float y, float z)
{
	preTransform("", Matrix::Rotate(x, y, z));
};

void Entity::unmangleVectors(Vector &a, Vector &b)
{
	int i;
	for (i=0; i<3; i++)
	{
		if (a[i] > b[i])
		{
			float tmp = a[i];
			a[i] = b[i];
			b[i] = tmp;
		};
	};
};

Entity::BoundingBox Entity::getBoundingBox()
{
	if (bbDirty)
	{
		Vector globMinVector = objects.begin()->second.matrix * objects.begin()->second.model->minVector;
		Vector globMaxVector = objects.begin()->second.matrix * objects.begin()->second.model->maxVector;
		unmangleVectors(globMinVector, globMaxVector);

		map<string, Object>::iterator it;
		for (it=objects.begin(); it!=objects.end(); ++it)
		{
			Vector minVector = it->second.matrix * it->second.model->minVector;
			Vector maxVector = it->second.matrix * it->second.model->maxVector;
			unmangleVectors(minVector, maxVector);

			int i;
			for (i=0; i<3; i++)
			{
				if (minVector[i] < globMinVector[i]) globMinVector[i] = minVector[i];
				if (maxVector[i] > globMaxVector[i]) globMaxVector[i] = maxVector[i];
			};
		};

		modelBoundingBox.min = modelMatrix * globMinVector;
		modelBoundingBox.max = modelMatrix * globMaxVector;
		unmangleVectors(modelBoundingBox.min, modelBoundingBox.max);
		bbDirty = false;
	};

	return modelBoundingBox;
};

Entity* Entity::checkCollision()
{
	vector<Entity*>::iterator it;
	for (it=World::entities.begin(); it!=World::entities.end(); ++it)
	{
		if ((*it) != this)
		{
			if (CollisionCheck::Entities(this, *it))
			{
				return *it;
			};
		};
	};

	return NULL;
};

Entity* Entity::move(Vector vec)
{
	translate(vec);
	Entity *coll = checkCollision();
	if (coll != NULL)
	{
		translate(-vec);
		return coll;
	};
	return NULL;
};
