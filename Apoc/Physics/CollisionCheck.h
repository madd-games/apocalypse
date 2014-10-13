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

#ifndef APOC_PHYSICS_COLLISION_CHECK_H
#define APOC_PHYSICS_COLLISION_CHECK_H

#include <Apoc/Math/Vector.h>
#include <Apoc/Math/Matrix.h>
#include <Apoc/Entity/Entity.h>
#include <Apoc/Entity/Model.h>

/**
 * \brief Utilities that help with collision checking.
 */
class CollisionCheck
{
public:
	/**
	 * \brief Tests segment-triangle intersection.
	 *
	 * Checks if segment (p<sub>0</sub>,p<sub>1</sub>) intersects with triangle (t<sub>0</sub>, t<sub>1</sub>, t<sub>2</sub>).
	 * If they do not, false is returned; if they do, then this function returns true and stores the point of
	 * intersection in i; otherwise, i is destroyed.
	 */
	static bool SegTri(Vector p0, Vector p1, Vector t0, Vector t1, Vector t2, Vector &i);
	static bool SegTri(Vector p0, Vector p1, Vector t0, Vector t1, Vector t2);

	/**
	 * \brief Checks if triangles A<sub>0,2</sub> and B<sub>0,2</sub> intersect.
	 */
	static bool TriTri(Vector a0, Vector a1, Vector a2, Vector b0, Vector b1, Vector b2);

	/**
	 * \brief Checks if 2 models are currently colliding when transformed by the specified matrices.
	 */
	static bool Models(Matrix &mat0, Model *mod0, Matrix &mat1, Model *mod1);

	/**
	 * \brief Checks if 2 entities are currently colliding.
	 */
	static bool Entities(Entity *a, Entity *b);
};

#endif
