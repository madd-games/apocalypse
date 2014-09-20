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
#include <math.h>

Matrix::Matrix()
{
};

Matrix::Matrix(const Matrix &mat)
{
	int i;
	for (i=0; i<4; i++)
	{
		columns[i] = mat.columns[i];
	};
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
	int i, j;
	for (i=0; i<4; i++)
	{
		for (j=0; j<4; j++)
		{
			out[i][j] = columns[j][i];
		};
	};
	
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
	Vector out;
	int i;
	for (i=0; i<4; i++)
	{
		out[i] = columns[i].dot(vec);
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
	Vector side = forward.cross(up).normalize();
	up = up.normalize();				// because some people are stupid :)
	
	Matrix mat;
	mat[0] = side;
	mat[1] = up;
	mat[2] = forward;
	mat[3] = eye;
	return mat;
};

Matrix Matrix::Perspective(float width, float height, float zNear, float zFar, float fov)
{
	float ar = width / height;
	float fac = 1.0/(ar*tanf(fov/2.0));
	
	Matrix out;
	out[0] = Vector(factor, 0.0, 0.0, 0.0);
	out[1] = Vector(0.0, factor, 0.0, 0.0);
	out[2] = Vector(0.0, 0.0, (-zNear-zFar)/(zNear-zFar), 1.0);
	out[3] = Vector(0.0, 0.0, (2*zFar*zNear)/(zNear-zFar), 0.0);
	
	return out;
};

ostream& operator<<(ostream &os, Matrix mat)
{
	int i, j;
	for (j=0; j<4; j++)
	{
		os << "[";
		
		for (i=0; i<4; i++)
		{
			os << mat[i][j] << "\t";
		};
		
		os << "]" << endl;
	};
	
	return os;
};
