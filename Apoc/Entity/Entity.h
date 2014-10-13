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

#ifndef APOC_ENTITY_ENTITY_H
#define APOC_ENTITY_ENTITY_H

#include <Apoc/Entity/Model.h>
#include <Apoc/Entity/Texture.h>
#include <Apoc/Math/Matrix.h>
#include <Apoc/Video/OpenGL.h>

#include <vector>
#include <map>
#include <string>

using namespace std;

/**
 * \brief Base class for all entities.
 */
class Entity
{
public:
	struct BoundingBox
	{
		Vector min;
		Vector max;
	};

private:
	struct Object
	{
		Model *model;
		// applied independently to the object.
		Matrix matrix;
		// Maps OpenGL texture units to the texture to be bound.
		map<unsigned int, Texture*> textures;
		Vector diffuseColor;
		Vector specularColor;
		float shininess;
	};

	// Maps object names to their descriptions.
	map<string, Object> objects;

	Matrix modelMatrix;
	BoundingBox modelBoundingBox;
	bool bbDirty;
	void unmangleVectors(Vector &a, Vector &b);

protected:
	/**
	 * \brief Transform the entity or a single object with a matrix.
	 *
	 * The new matrix for the object or entity is mat multiplied by the old matrix.
	 * \param obj If this is an empty string, the transformation is applied to the overall entity; otherwise, this is the
	 *            name of the object to apply this transformation to.
	 * \param mat The transformation matrix.
	 */
	virtual void transform(string obj, Matrix mat);

	/**
	 * \brief Like transform, except the transformation is applied before all others.
	 * \sa transform
	 */
	virtual void preTransform(string obj, Matrix mat);

public:
	/**
	 * \brief Constructor.
	 * \param defs The model description.
	 */
	Entity(Model::ObjDef *defs);
	
	/**
	 * \brief Virtual destructor.
	 */
	virtual ~Entity();
	
	/**
	 * \brief Called to update the entity.
	 * 
	 * This function is called in an infinite loop while an entity is in the scene.
	 * It does nothing by default.
	 */
	virtual void update();

	/**
	 * \brief Renders the entity. Do not override.
	 */
	void renderObjects();

	/**
	 * \brief Translate the entity.
	 */
	virtual void translate(Vector vec);

	/**
	 * \brief Rotate the entity.
	 * \sa Matrix::Rotate
	 */
	virtual void rotate(float x, float y, float z);

	/**
	 * \brief Get the model's bounding box.
	 */
	BoundingBox getBoundingBox();

	/**
	 * \brief Checks if this entity is colliding with another one.
	 *
	 * If there is a collision, then the colliding entity is returned, otherwise, NULL is returned.
	 */
	Entity *checkCollision();

	/**
	 * \brief Translate the entity with collision checking.
	 *
	 * If this move causes the entity to collide with something, it will not occur.
	 * \return NULL on successful move, the colliding entity in case of collision.
	 */
	Entity* move(Vector vec);

	friend class World;
	friend class CollisionCheck;
};

#endif
