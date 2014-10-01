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

#ifndef APOC_MATH_VECTOR_H
#define APOC_MATH_VECTOR_H

#include <iostream>

using namespace std;

/**
 * \brief 4D homogenous vector class.
 */
class Vector
{
private:
	float coords[4];
	
public:
	Vector();
	Vector(float x, float y);
	Vector(float x, float y, float z);
	Vector(float x, float y, float z, float w);
	Vector(const Vector &vec);
	
	float& x();
	float& y();
	float& z();
	float& w();
	
	float& operator[](int i);
	Vector& operator=(Vector vec);
	
	Vector operator+(Vector b);
	Vector operator-(Vector b);
	Vector operator*(Vector b);
	Vector operator/(Vector b);
	Vector operator*(float x);
	Vector operator-();

	bool operator==(const Vector b);
	bool operator!=(const Vector b);

	/**
	 * \brief Returns the dot product of this vector and vector B.
	 */
	float dot(Vector b);
	
	/**
	 * \brief Returns the cross product of this vector and vector B.
	 */
	Vector cross(Vector b);
	
	/**
	 * \brief Normalizes the vector.
	 * \return A vector with the same direction as this one, but a magnitude of 1.
	 */
	Vector normalize();
	
	/**
	 * \brief Retrurns the length (magnitude) of this vector.
	 */
	float length();

	/**
	 * \brief Performs the projection divide (divide by w) and returns the result.
	 */
	Vector project();
	
	friend ostream& operator<<(ostream &os, Vector vec);
};

ostream& operator<<(ostream &os, Vector vec);

#endif
