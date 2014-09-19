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
