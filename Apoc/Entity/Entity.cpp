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

Entity::Entity(Model::ObjDef *defs) : bbDirty(true), shouldRemove(false)
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
		string normalMap = defs->normalMap;
		if (texName != "empty_texture")
		{
			obj.textures[0] = Texture::Get(texName);
		};
		if (specMap != "<NONE>")
		{
			obj.textures[4] = Texture::Get(specMap);
		};
		if (normalMap != "<NONE>")
		{
			obj.textures[5] = Texture::Get(normalMap);
		};
		obj.matrix = Matrix::Identity();
		obj.diffuseColor = defs->diffuseColor;
		obj.specularColor = defs->specularColor;
		obj.shininess = defs->shininess;
		string name(defs->name);
		obj.visible = (name != "ApocColl");
		obj.collideable = (name == "ApocColl");
		objects[defs->name] = obj;
		defs++;
	};

	if (objects.count("ApocColl") == 0)
	{
		map<string, Object>::iterator it;
		for (it=objects.begin(); it!=objects.end(); ++it)
		{
			it->second.collideable = true;
		};
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

Matrix Entity::getModelMatrix()
{
	return modelMatrix;
};

bool& Entity::visible(string name)
{
	return objects[name].visible;
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
		if (it->second.visible)
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
};

void Entity::translate(Vector vec, const char *objName)
{
	transform(objName, Matrix::Translate(vec.x(), vec.y(), vec.z()));
};

void Entity::rotate(float x, float y, float z, const char *objName)
{
	preTransform(objName, Matrix::Rotate(x, y, z));
};

void Entity::markForRemoval()
{
	shouldRemove = true;
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
		modelBoundingBox.min = modelMatrix * objects.begin()->second.matrix * objects.begin()->second.model->data[0].pos;
		modelBoundingBox.max = modelBoundingBox.min;

		map<string, Object>::iterator it;
		for (it=objects.begin(); it!=objects.end(); ++it)
		{
			Matrix mat = modelMatrix * it->second.matrix;
			int i;
			for (i=0; i<it->second.model->vertexCount; i++)
			{
				Vector pos = mat * it->second.model->data[i].pos;
				int j;
				for (j=0; j<3; j++)
				{
					if (pos[j] < modelBoundingBox.min[j]) modelBoundingBox.min[j] = pos[j];
					if (pos[j] > modelBoundingBox.max[j]) modelBoundingBox.max[j] = pos[j];
				};
			};
		};

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
