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
#include <Apoc/Utils/Archive.h>
#include <string.h>
#include <iostream>
#include <stddef.h>

using namespace std;

extern RenderHandler *apocRenderHandler;
map<string, map<string, Entity::Object> > Entity::apocEntityCache;

// XXX: Don't worry guys, this constructor is going away soon.
Entity::Entity(Model::ObjDef *defs) : bbDirty(true), shouldRemove(false), isStatic(true), entParent(NULL)
{
	cerr << "[APOC] [WARNING] The deprecated Entity constructor was used to load an embedded OBJ file!" << endl;

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

Entity::Entity(string entname) : bbDirty(true), shouldRemove(false), isStatic(true), entParent(NULL)
{
	modelMatrix = Matrix::Identity();

	if (apocEntityCache.count(entname) != 0)
	{
		objects = apocEntityCache[entname];
	}
	else
	{
		DataFile file(string("Game/Models/") + entname + ".apm");
		uint8_t *data = new uint8_t[file.getSize()];
		file.read(data, file.getSize());

		APM_Header *header = (APM_Header*) data;
		if (memcmp(header->magic, "APM", 3) != 0)
		{
			ApocFail(string(entname) + " does not have a valid APM file");
		};

		map<uint32_t, Texture*> modelTextures;
		uint32_t count = header->numTex;
		APM_TexHeader *texHeader = (APM_TexHeader*) &data[header->offTex];

		while (count--)
		{
			uint8_t *texdata = (uint8_t*) (texHeader+1);
			modelTextures[texHeader->idx] = new Texture(texHeader->width, texHeader->height, texdata, (texHeader->flags & 1) == 0);
			texHeader = (APM_TexHeader*) &texdata[4 * texHeader->width * texHeader->height];
		};

		count = header->numObjDefs;
		uint8_t *objscan = &data[header->offObjDefs];

		while (count--)
		{
			string name((const char*)objscan);
			objscan += name.size();
			objscan++;
			APM_ObjHeader *objHeader = (APM_ObjHeader*) objscan;

			// unroll the vertices
			Model::Vertex *vertices = new Model::Vertex[objHeader->numVertices];
			Model::Vertex *vunroll = vertices;
			uint8_t *mdlv = (uint8_t*) objHeader + objHeader->szThis;
			uint32_t vcnt = objHeader->numVertices;

			while (vcnt--)
			{
				memcpy(vunroll, mdlv, header->szVertex);
				vunroll++;
				mdlv += header->szVertex;
			};

			Object obj;
			obj.model = new Model(vertices, objHeader->numVertices);
			obj.matrix = Matrix::Identity();
			if (objHeader->idxColor != 0)
			{
				obj.textures[0] = modelTextures[objHeader->idxColor];
			};
			if (objHeader->idxSpecular != 0)
			{
				obj.textures[4] = modelTextures[objHeader->idxSpecular];
			};
			if (objHeader->idxNormals != 0)
			{
				obj.textures[5] = modelTextures[objHeader->idxNormals];
			};
			obj.diffuseColor = Vector(
				(float) objHeader->colDiffuse[0] / 255.0,
				(float) objHeader->colDiffuse[1] / 255.0,
				(float) objHeader->colDiffuse[2] / 255.0,
				(float) objHeader->colDiffuse[3] / 255.0
			);
			obj.specularColor = Vector(
				(float) objHeader->colSpecular[0] / 255.0,
				(float) objHeader->colSpecular[1] / 255.0,
				(float) objHeader->colSpecular[2] / 255.0,
				(float) objHeader->colSpecular[3] / 255.0
			);
			obj.shininess = objHeader->shininess;
			obj.visible = (name != "ApocColl");
			obj.collideable = (name == "ApocColl");
			objects[name] = obj;

			objscan = mdlv;
		};

		delete data;

		if (objects.count("ApocColl") == 0)
		{
			map<string, Object>::iterator it;
			for (it=objects.begin(); it!=objects.end(); ++it)
			{
				it->second.collideable = true;
			};
		};

		apocEntityCache[entname] = objects;
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
	isStatic = false;
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
	isStatic = false;
};

Matrix Entity::getModelMatrix()
{
	if (entParent != NULL)
	{
		return entParent->getModelMatrix() * modelMatrix;
	};
	return modelMatrix;
};

bool& Entity::visible(string name)
{
	return objects[name].visible;
};

bool& Entity::collideable(string name)
{
	return objects[name].collideable;
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

	Matrix matModel = getModelMatrix();
	glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, &matModel[0][0]);

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

void Entity::attachTo(Entity *parent)
{
	entParent = parent;
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
