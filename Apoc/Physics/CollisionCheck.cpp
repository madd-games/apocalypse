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

#include <Apoc/Physics/CollisionCheck.h>
#include <math.h>

bool CollisionCheck::SegTri(Vector p0, Vector p1, Vector t0, Vector t1, Vector t2, Vector &i)
{
	// This function was translated from:
	// http://geomalgorithms.com/a06-_intersect-2.html#intersect_SegTriangle()
	// I shall remove this comment as soon as I understand 100% of this :D

	Vector u, v, n;				// triangle vectors
	Vector dir, w0, w;			// ray vectors
	float r, a, b;				// params to calc ray-plane intersect

	// get triangle edge vectors and plane normal
	u = t1 - t0;
	v = t2 - t0;
	n = u.cross(v);
	if (n == Vector::Zero)
	{
		return false;
	};

	dir = p1 - p0;				// ray direction vector
	w0 = p0 - t0;
	a = -n.dot(w0);
	b = n.dot(dir);
	if (fabs(b) < 0.00001)			// ray is parallel to triangle plane
	{
		return false;
	};

	// get intersection point of ray with triangle
	r = a / b;
	if (r < 0)
	{
		// ray is going away from triangle.
		return false;
	};
	if (r > 1)
	{
		// unsure about this one
		return false;
	};

	i = p0 + dir * r;

	// is i inside t?
	float uu, uv, vv, wu, wv, D;
	uu = u.dot(u);
	uv = u.dot(v);
	vv = v.dot(v);
	w = i - t0;
	wu = w.dot(u);
	wv = w.dot(v);
	D = uv * uv - uu * vv;

	// get and test parametric coords
	float s, t;
	s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0)			// i is outside t
	{
		return false;
	};

	t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || t > 1.0)
	{
		return false;
	};

	// finally :D
	return true;
};

bool CollisionCheck::SegTri(Vector p0, Vector p1, Vector t0, Vector t1, Vector t2)
{
	Vector ignore;
	return SegTri(p0, p1, t0, t1, t2, ignore);
};

bool CollisionCheck::TriTri(Vector a0, Vector a1, Vector a2, Vector b0, Vector b1, Vector b2)
{
	// fails in some cases FIXME
	// (sometimes reports true when it shouldn't)
	// a temporary fix is here but we should probably find better ways of
	// collision checking anyway.

	return	(	SegTri(a0, a1, b0, b1, b2)
		||	SegTri(a1, a2, b0, b1, b2)
		||	SegTri(a2, a0, b0, b1, b2))
		&&
		(
			SegTri(b0, b1, a0, a1, a2)
		||	SegTri(b1, b2, a0, a1, a2)
		||	SegTri(b2, b0, a0, a1, a2)
		);
};

bool CollisionCheck::Models(Matrix &mat0, Model *mod0, Matrix &mat1, Model *mod1, bool trans)
{
	int i, j;
	for (i=0; i<mod0->vertexCount; i+=3)
	{
		Vector a0, a1, a2;
		if (trans)
		{
			a0 = mat0 * mod0->data[i+0].pos;
			a1 = mat0 * mod0->data[i+1].pos;
			a2 = mat0 * mod0->data[i+2].pos;
		}
		else
		{
			a0 = mod0->data[i+0].pos;
			a1 = mod0->data[i+1].pos;
			a2 = mod0->data[i+2].pos;
		};

		for (j=0; j<mod1->vertexCount; j+=3)
		{
			Vector b0, b1, b2;
			if (trans)
			{
				b0 = mat1 * mod1->data[j+0].pos;
				b1 = mat1 * mod1->data[j+1].pos;
				b2 = mat1 * mod1->data[j+2].pos;
			}
			else
			{
				b0 = mod1->data[j+0].pos;
				b1 = mod1->data[j+1].pos;
				b2 = mod1->data[j+2].pos;
			};

			if (TriTri(a0, a1, a2, b0, b1, b2))
			{
#if 0
				cout << "Triangles colliding: " << endl;
				cout << "First: " << endl;
				cout << "\t" << a0 << endl;
				cout << "\t" << a1 << endl;
				cout << "\t" << a2 << endl;
				cout << "Second: " << endl;
				cout << "\t" << b0 << endl;
				cout << "\t" << b1 << endl;
				cout << "\t" << b2 << endl;
#endif
				return true;
			};
		};
	};

	return false;
};

bool CollisionCheck::Entities(Entity *a, Entity *b)
{
	// check if the bounding boxes intersect first, as this is more efficient.
	Entity::BoundingBox bbA = a->getBoundingBox();
	Entity::BoundingBox bbB = b->getBoundingBox();

	int i;
	for (i=0; i<3; i++)
	{
		if ((bbA.max[i] < bbB.min[i]) || (bbA.min[i] > bbB.max[i])) return false;
	};

	// bounding boxes intersect, check all the triangles now.
	map<string, Entity::Object>::iterator itA;
	for (itA=a->objects.begin(); itA!=a->objects.end(); ++itA)
	{
		if (itA->second.collideable)
		{
			Matrix mat0 = a->getModelMatrix() * itA->second.matrix;
			Model *mod0 = itA->second.model;

			map<string, Entity::Object>::iterator itB;
			for (itB=b->objects.begin(); itB!=b->objects.end(); ++itB)
			{
				if (itB->second.collideable)
				{
					Matrix mat1;
					mat1 = b->getModelMatrix() * itB->second.matrix;
					Model *mod1 = itB->second.model;

					if (Models(mat0, mod0, mat1, mod1, true))
					{
						//cout << "COLLISION between " << itA->first << " and " << itB->first << endl;
						return true;
					};
				};
			};
		};
	};

	return false;
};
