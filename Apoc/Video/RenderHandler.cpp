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

#include <Apoc/Video/RenderHandler.h>
#include <Apoc/Utils/Utils.h>

#include <stdlib.h>
#include <sstream>

using namespace std;

GLuint RenderHandler::createProgram(string glslVertex, string glslFragment)
{
	GLuint program = glCreateProgram();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	const GLchar *codeVertex = (const GLchar*) glslVertex.c_str();
	const GLchar *codeFragment = (const GLchar*) glslFragment.c_str();
	
	glShaderSource(vertexShader, 1, &codeVertex, NULL);
	glShaderSource(fragmentShader, 1, &codeFragment, NULL);
	
	glCompileShader(vertexShader);
	GLint status, logSize;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logSize);
		char *buffer = new char[logSize];
		glGetShaderInfoLog(vertexShader, logSize, NULL, buffer);
		
		stringstream ss;
		ss << "Compilation of the vertex shader failed:" << endl;
		ss << buffer;
		ApocFail(ss.str());
	};
	
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logSize);
		char *buffer = new char[logSize];
		glGetShaderInfoLog(fragmentShader, logSize, NULL, buffer);
		
		stringstream ss;
		ss << "Compilation of the fragment shader failed:" << endl;
		ss << buffer;
		ApocFail(ss.str());
	};
	
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status)
	{
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		char *buffer = new char[logSize];
		glGetProgramInfoLog(program, logSize, NULL, buffer);
		
		stringstream ss;
		ss << "Linking the shader program failed:" << endl;
		ss << buffer;
		ApocFail(ss.str());
	};
	
	return program;
};

RenderHandler::RenderHandler()
{
};

RenderHandler::~RenderHandler()
{
};
