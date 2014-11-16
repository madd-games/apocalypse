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

#include <inttypes.h>

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
	struct APM_Header
	{
		char     magic[3];
		uint8_t  rev;
		uint32_t offObjDefs;
		uint32_t numObjDefs;
		uint32_t szVertex;
		uint32_t offTex;
		uint32_t numTex;
	} __attribute__ ((packed));

	struct APM_ObjHeader
	{
		uint32_t numVertices;
		uint32_t idxColor;
		uint32_t idxSpecular;
		uint32_t idxNormals;
		uint8_t  colDiffuse[4];
		uint8_t  colSpecular[4];
		float    shininess;
		uint32_t szThis;
		uint32_t idxIllumMap;		// only if (szThis >= 36)
		uint32_t idxWarpMap;		// only if (szThis >= 40)
	} __attribute__ ((packed));

	struct APM_TexHeader
	{
		uint32_t idx;
		uint32_t width;
		uint32_t height;
		uint16_t flags;
	} __attribute__ ((packed));

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
		bool visible;
		bool collideable;		// true if this is included as a collision model.
	};

	// A cache to store all loaded entities (from APM files).
	static map<string, map<string, Object> > apocEntityCache;

	// Maps object names to their descriptions.
	map<string, Object> objects;

	Matrix modelMatrix;
	BoundingBox modelBoundingBox;
	bool bbDirty;
	void unmangleVectors(Vector &a, Vector &b);

	// true if the entity is to be removed after the next update.
	bool shouldRemove;

	bool isStatic;

	// the parent entity (a child is always in it's parent's model space).
	Entity *entParent;

protected:
	/**
	 * \brief Transform the entity or a single object with a matrix.
	 *
	 * The new matrix for the object or entity is mat multiplied by the old matrix.
	 * \param obj If this is an empty string, the transformation is applied to the overall entity; otherwise, this is the
	 *            name of the object to apply this transformation to.
	 * \param mat The transformation matrix.
	 */
	void transform(string obj, Matrix mat);

	/**
	 * \brief Like transform, except the transformation is applied before all others.
	 * \sa transform
	 */
	void preTransform(string obj, Matrix mat);

	/**
	 * \brief Returns the model matrix.
	 */
	Matrix getModelMatrix();

	/**
	 * \brief Access an object's visibility property.
	 * \param name The name of the object.
	 *
	 * This function returns a reference to a variable, which when set to true causes the object to be
	 * visible (the default), whereas when set to false, the object will not be drawn.
	 */
	bool& visible(string name);

	/**
	 * \brief Access an object's collideability property.
	 * \param name The name of the object.
	 *
	 * This function returns a reference to a variable, which when set to true causes the object to be
	 * collideable (the default), whereas when set to false, the object will not be collision tested.
	 */
	bool& collideable(string name);

	/**
	 * \brief Translate the entity with collision checking.
	 *
	 * If this move causes the entity to collide with something, it will not occur.
	 * \return NULL on successful move, the colliding entity in case of collision.
	 */
	Entity* move(Vector vec);

	/**
	 * \brief Translate the entity.
	 */
	void translate(Vector vec, const char *objName = "");

	/**
	 * \brief Rotate the entity.
	 * \sa Matrix::Rotate
	 */
	void rotate(float x, float y, float z, const char *objName = "");

	/**
	 * \brief Mark the entity for removal.
	 *
	 * The entity will actually be deleted after the next call to update() - if it is marked
	 * for removal during a call to update(), then it will be deleted as soon as update() returns.
	 */
	void markForRemoval();

	/**
	 * \brief Attach this entity to a parent.
	 *
	 * This means that this entity will follow its parent and orbit it as it rotates, as if this whole
	 * entity were an object within the parent.
	 */
	void attachTo(Entity *parent);

public:
	/**
	 * \brief Deprecated constructor.
	 * \param defs The model description.
	 * \deprecated Use the other constructor because OBJ files will soon not be supported!
	 */
	Entity(Model::ObjDef *defs);

	/**
	 * \brief Constructor.
	 * \param name Name of the entity.
	 *
	 * The model file is "Game/Models/<name>.apm". ApocFail() is raised if the model file
	 * does not exist or is invalid.
	 */
	Entity(string entname);

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
	 * \brief Get the model's bounding box.
	 */
	BoundingBox getBoundingBox();

	/**
	 * \brief Checks if this entity is colliding with another one.
	 *
	 * If there is a collision, then the colliding entity is returned, otherwise, NULL is returned.
	 */
	Entity *checkCollision();

	friend class World;
	friend class CollisionCheck;
};

#endif
