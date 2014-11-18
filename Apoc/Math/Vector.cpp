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

#include <Apoc/Math/Vector.h>
#include <math.h>

#ifdef __GNUC__
Vector::Mask Vector::MaskYZX = {1, 2, 0, 3};
Vector::Mask Vector::MaskZXY = {2, 0, 1, 3};
#endif

Vector Vector::Zero(0, 0, 0, 0);
Vector Vector::Origin(0, 0, 0, 1);

Vector::Vector()
{
	coords[0] = 0.0;
	coords[1] = 0.0;
	coords[2] = 0.0;
	coords[3] = 1.0;
};

Vector::Vector(float x, float y)
{
	coords[0] = x;
	coords[1] = y;
	coords[2] = 0.0;
	coords[3] = 1.0;
};

Vector::Vector(float x, float y, float z)
{
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
	coords[3] = 1.0;
};

Vector::Vector(float x, float y, float z, float w)
{
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
	coords[3] = w;
};

Vector::Vector(const Vector &vec)
{
	int i;
	for (i=0; i<4; i++)
	{
		coords[i] = vec.coords[i];
	};
};

float& Vector::x()
{
	return coords[0];
};

float& Vector::y()
{
	return coords[1];
};

float& Vector::z()
{
	return coords[2];
};

float& Vector::w()
{
	return coords[3];
};

float& Vector::operator[](int i)
{
	return coords[i];
};

Vector& Vector::operator=(Vector vec)
{
#ifdef __GNUC__
	coords = vec.coords;
#else
	int i;
	for (i=0; i<4; i++)
	{
		coords[i] = vec[i];
	};
#endif
	return *this;
};

Vector Vector::operator+(Vector b)
{
	Vector out;
#ifdef __GNUC__
	out.coords = coords + b.coords;
#else
	int i;
	for (i=0; i<4; i++)
	{
		out[i] = coords[i] + b[i];
	};
#endif
	return out;
};

Vector Vector::operator-(Vector b)
{
	Vector out;
#ifdef __GNUC__
	out.coords = coords - b.coords;
#else
	int i;
	for (i=0; i<4; i++)
	{
		out[i] = coords[i] - b[i];
	};
#endif
	return out;
};

Vector Vector::operator*(Vector b)
{
	Vector out;
#ifdef __GNUC__
	out.coords = coords * b.coords;
#else
	int i;
	for (i=0; i<4; i++)
	{
		out[i] = coords[i] * b[i];
	};
#endif
	return out;
};

Vector Vector::operator/(Vector b)
{
	Vector out;
#ifdef __GNUC__
	out.coords = coords / b.coords;
#else
	int i;
	for (i=0; i<4; i++)
	{
		out[i] = coords[i] / b[i];
	};
#endif
	return out;
};

Vector Vector::operator*(float x)
{
#ifdef __GNUC__
	Vector out;
	out.coords = coords * x;
	return out;
#else
	return Vector(coords[0]*x, coords[1]*x, coords[2]*x, coords[3]*x);
#endif
};

Vector Vector::operator-()
{
#ifdef __GNUC__
	Vector out;
	out.coords = -coords;
	return out;
#else
	return Vector(-coords[0], -coords[1], -coords[2], coords[3]);
#endif
};

bool Vector::operator==(const Vector b)
{
	int i;
	for (i=0; i<4; i++)
	{
		if (coords[i] != b.coords[i])
		{
			return false;
		};
	};

	return true;
};

bool Vector::operator!=(const Vector b)
{
	int i;
	for (i=0; i<4; i++)
	{
		if (coords[i] == b.coords[i])
		{
			return false;
		};
	};

	return true;
};

float Vector::dot(Vector b)
{
#ifdef __GNUC__
	NativeVector v = coords * b.coords;
	return v[0] + v[1] + v[2] + v[3];
#else
	float out = 0.0;
	int i;
	for (i=0; i<4; i++)
	{
		out += coords[i] * b[i];
	};

	return out;
#endif
};

Vector Vector::cross(Vector b)
{
	Vector out;
#ifdef __GNUC__
	out.coords = (__builtin_shuffle(coords, MaskYZX) * __builtin_shuffle(b.coords, MaskZXY))
			- (__builtin_shuffle(coords, MaskZXY) * __builtin_shuffle(b.coords, MaskYZX));
#else
	out[0] = coords[1]*b[2] - coords[2]*b[1];		// X
	out[1] = coords[2]*b[0] - coords[0]*b[2];		// Y
	out[2] = coords[0]*b[1] - coords[1]*b[0];		// Z
#endif
	out[3] = 0.0;
	return out;
};

Vector Vector::normalize()
{
	// Don't calculate the length 3 times!
	float l = length();
	return Vector(x()/l, y()/l, z()/l, 0.0);
};

float Vector::length()
{
	return sqrt(x()*x() + y()*y() + z()*z());
};

Vector Vector::project()
{
	if (w() == 0) return *this;
	return Vector(x()/w(), y()/w(), z()/w(), 1.0);
};

ostream& operator<<(ostream &os, Vector vec)
{
	os << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ", " << vec.w() << ")";
	return os;
};

Vector operator*(float x, Vector vec)
{
	return vec * x;
};
