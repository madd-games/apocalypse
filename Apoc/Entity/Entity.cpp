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

Entity::Entity(Model::ObjDef *defs)
{
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
		objects[defs->name] = obj;
		defs++;
	};
};

Entity::~Entity()
{
};

void Entity::update()
{
};

void Entity::renderObjects()
{
	map<string, Object>::iterator it;
	for (it=objects.begin(); it!=objects.end(); ++it)
	{
		map<unsigned int, Texture*>::iterator jt;
		for (jt=it->second.textures.begin(); jt!=it->second.textures.end(); ++jt)
		{
			glActiveTexture(GL_TEXTURE0 + jt->first);
			jt->second->bind();
		};

		it->second.model->draw();
	};
};
