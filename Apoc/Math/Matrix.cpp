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

#include <Apoc/Math/Matrix.h>
#include <Apoc/Utils/Utils.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

#ifdef __GNUC__
Matrix::Mask Matrix::MaskTranspose = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};
#endif

float Matrix::Minor(Matrix &mat, int order)
{
	if (order == 2)
	{
		return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
	};

	Matrix submat;
	int i, j, k=0, l;
	float d = 0;
	for (i=0; i<order; i++)
	{
		k = 0;
		for (j=0; j<order; j++)
		{
			if (i != j)
			{
				for (l=1; l<order; l++)
				{
					submat[k][l-1] = mat[j][l];
				};
				k++;
			};
		};
		d += pow(-1, i) * mat[i][0] * Minor(submat, order-1);
	};
	return d;
};

Matrix::Matrix()
{
#ifdef __GNUC__
	natColumns = (NativeMatrix*) &columns[0][0];
#endif
};

Matrix::Matrix(const Matrix &mat)
{
	int i;
	for (i=0; i<4; i++)
	{
		columns[i] = mat.columns[i];
	};
#ifdef __GNUC__
	natColumns = (NativeMatrix*) &columns[0][0];
#endif
};

Matrix& Matrix::operator=(Matrix mat)
{
	int i;
	for (i=0; i<4; i++)
	{
		columns[i] = mat[i];
	};
	
	return *this;
};

Vector& Matrix::operator[](int i)
{
	return columns[i];
};

Matrix Matrix::transpose()
{
	Matrix out;
#ifdef __GNUC__
	*out.natColumns = __builtin_shuffle(*natColumns, MaskTranspose);
#else
	int i, j;
	for (i=0; i<4; i++)
	{
		for (j=0; j<4; j++)
		{
			out[i][j] = columns[j][i];
		};
	};
#endif
	
	return out;
};

Matrix Matrix::operator*(Matrix b)
{
	Matrix a = transpose();
	Matrix out;
	
	int i, j;
	for (i=0; i<4; i++)
	{
		for (j=0; j<4; j++)
		{
			out[i][j] = a[j].dot(b[i]);
		};
	};
	
	return out;
};

Vector Matrix::operator*(Vector vec)
{
	Matrix a = transpose();
	Vector out;
	int i;
	for (i=0; i<4; i++)
	{
		out[i] = a[i].dot(vec);
	};
	return out;
};

float Matrix::det()
{
	return Minor(*this, 4);
};

Matrix Matrix::inv()
{
	float d = det();
	if (d == 0.0)
	{
		ApocFail("Attempting to find the inverse of a matrix with a determinant of zero.");
	};
	float factor = 1.0 / d;

	Matrix out;
	int i, j;
	for (i=0; i<4; i++)
	{
		for (j=0; j<4; j++)
		{
			Matrix submat;		// the matrix for which we are finding the determinant.
			int k, l, m=0, n;
			for (k=0; k<4; k++)
			{
				if (k != i)
				{
					n = 0;
					for (l=0; l<4; l++)
					{
						if (l != j)
						{
							submat[m][n] = (*this)[k][l];
							n++;
						};
					};
					m++;
				};
			};

			// minors, cofactors, transpose and divide by determinant in one go
			// i don't care if it's confusing, it's a game engine that has to perform well,
			// not a maths lesson.
			out[j][i] = factor * pow(-1, i+j) * Minor(submat, 3);
		};
	};

	return out;
};

Matrix Matrix::Identity()
{
	Matrix out;
	int i, j;
	for (i=0; i<4; i++)
	{
		for (j=0; j<4; j++)
		{
			if (i == j)
			{
				out[i][j] = 1;
			}
			else
			{
				out[i][j] = 0;
			};
		};
	};
	
	return out;
};

Matrix Matrix::Scale(float x, float y, float z)
{
	Matrix mat;
	mat[0] = Vector(x, 0.0, 0.0, 0.0);
	mat[1] = Vector(0.0, y, 0.0, 0.0);
	mat[2] = Vector(0.0, 0.0, z, 0.0);
	mat[3] = Vector(0.0, 0.0, 0.0, 1.0);
	return mat;
};

Matrix Matrix::Translate(float x, float y, float z)
{
	Matrix mat = Identity();
	mat[3] = Vector(x, y, z, 1.0);
	return mat;
};

Matrix Matrix::LookAt(Vector eye, Vector up, Vector ref)
{
	Vector forward = (ref - eye).normalize();
	Vector right = up.cross(forward).normalize();
	up = forward.cross(right);
	
	Matrix mat;
	mat[0] = Vector(right[0], up[0], forward[0], 0.0);
	mat[1] = Vector(right[1], up[1], forward[1], 0.0);
	mat[2] = Vector(right[2], up[2], forward[2], 0.0);
	mat[3] = Vector(0, 0, 0, 1);

	return mat * Translate(-eye.x(), -eye.y(), -eye.z());
};

Matrix Matrix::Perspective(float width, float height, float zNear, float zFar, float fov)
{
	float ar = width / height;
	float fac = 1.0/(tanf(fov/2.0));
	
	Matrix out;
	out[0] = Vector(fac/ar, 0.0, 0.0, 0.0);
	out[1] = Vector(0.0, fac, 0.0, 0.0);
	out[2] = Vector(0.0, 0.0, (-zNear-zFar)/(zNear-zFar), 1.0);
	out[3] = Vector(0.0, 0.0, (2*zFar*zNear)/(zNear-zFar), 0.0);
	
	return out;
};

Matrix Matrix::Ortho(float left, float right, float top, float bottom, float zNear, float zFar)
{
	Matrix mat = Identity();
	mat[0][0] = 2.0/(right-left);
	mat[1][1] = 2.0/(top-bottom);
	mat[2][2] = -2.0/(zFar-zNear);
	mat[3] = Vector(-(right+left)/(right-left), -(top+bottom)/(top-bottom), -(zFar+zNear)/(zFar-zNear), 1.0);
	return mat;
};

Matrix Matrix::Rotate(float x, float y, float z)
{
	Matrix matX;
	matX[0] = Vector(1, 0, 0, 0);
	matX[1] = Vector(0, cos(x), -sin(x), 0);
	matX[2] = Vector(0, sin(x), cos(x), 0);
	matX[3] = Vector(0, 0, 0, 1);

	Matrix matY;
	matY[0] = Vector(cos(y), 0, -sin(y), 0);
	matY[1] = Vector(0, 1, 0, 0);
	matY[2] = Vector(sin(y), 0, cos(y), 0);
	matY[3] = Vector(0, 0, 0, 1);

	Matrix matZ;
	matZ[0] = Vector(cos(z), sin(z), 0, 0);
	matZ[1] = Vector(-sin(z), cos(z), 0, 0);
	matZ[2] = Vector(0, 0, 1, 0);
	matZ[3] = Vector(0, 0, 0, 1);

	return matX * matY * matZ;
};

ostream& operator<<(ostream &os, Matrix mat)
{
	int i, j;
	for (j=0; j<4; j++)
	{
		os << "[";
		
		for (i=0; i<4; i++)
		{
			os << mat[i][j];
			if (i != 3) os << "\t\t";
		};
		
		os << "]" << endl;
	};
	
	return os;
};

Matrix operator*(float x, Matrix mat)
{
	Matrix out;
	int i, j;
	for (i=0; i<4; i++)
	{
		for (j=0; j<4; i++)
		{
			out[i][j] = x * mat[i][j];
		};
	};
};
