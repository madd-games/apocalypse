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

#ifndef APOC_MATH_MATRIX_H
#define APOC_MATH_MATRIX_H

#include <Apoc/Math/Vector.h>
#include <iostream>

using namespace std;

/**
 * \brief 4x4 matrix class.
 */
class Matrix
{
private:
	Vector columns[4];
	static float Minor(Matrix &mat, int order);

public:
	Matrix();
	Matrix(const Matrix &mat);
	Matrix& operator=(Matrix mat);
	Vector& operator[](int i);
	Matrix operator*(Matrix b);
	Vector operator*(Vector vec);

	/**
	 * \brief Returns the determinant of this matrix.
	 */
	float det();

	/**
	 * \brief Returns the inverse of this matrix.
	 *
	 * If the determinant (det) of the matrix is zero, then there is no inverse and
	 * ApocFail() is thrown.
	 */
	Matrix inv();

	/**
	 * \brief Returns the transpose of this matrix.
	 */
	Matrix transpose();

	/**
	 * \brief Returns the identity matrix.
	 */
	static Matrix Identity();
	
	/**
	 * \brief Returns a scaling transformation matrix.
	 */
	static Matrix Scale(float x, float y, float z);
	
	/**
	 * \brief Returns a translating transformation matrix.
	 */
	static Matrix Translate(float x, float y, float z);
	
	/**
	 * \brief Returns a look-at matrix.
	 * \param eye The position of the eye.
	 * \param up The up vector (which direction is up). w must be 0.
	 * \param ref The point of reference, which the eye is looking directly at.
	 */
	static Matrix LookAt(Vector eye, Vector up, Vector ref);
	
	/**
	 * \brief Returns a perspective projection matrix.
	 * \param width The screen width.
	 * \param height The screen height.
	 * \param zNear The near Z value (so close that we can't see).
	 * \param zFar The far Z value (so far that we can't see).
	 * \param fov The field of view (in radians).
	 */
	static Matrix Perspective(float width, float height, float zNear, float zFar, float fov);

	/**
	 * \brief Returns an orthographic projection matrix.
	 * \param left The left coordinate.
	 * \param right The right coordinate.
	 * \param top The top coordinate.
	 * \param bottom The bottom coordinate.
	 * \param zNear The near Z coordinate.
	 * \param zFar The far Z coordinate.
	 */
	static Matrix Ortho(float left, float right, float top, float bottom, float zNear, float zFar);

	/**
	 * \brief Returns a rotation matrix.
	 * \param x The angle to rotate by around the X axis (in radians).
	 * \param y The angle to rotate by around the Y axis (in radians).
	 * \param z The angle to rotate by around the Z axis (in radians).
	 */
	static Matrix Rotate(float x, float y, float z);

	friend ostream& operator<<(ostream &os, Matrix mat);
};

ostream& operator<<(ostream &os, Matrix mat);
Matrix operator*(float x, Matrix mat);

#endif
