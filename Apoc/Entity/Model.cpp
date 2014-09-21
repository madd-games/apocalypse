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

#include <Apoc/Entity/Model.h>
#include <Apoc/Utils/Utils.h>

Model::Model(const Model::Vertex *vertices, const int count) : vertexCount(count)
{
	if (count == 0)
	{
		ApocFail("Empty model specified!");
	};

	if ((count % 3) != 0)
	{
		ApocFail("The specified model is not triangulated!");
	};

	GLint attrVertex, attrTexCoords, attrNormal;
	apocRenderHandler->getAttrLocations(attrVertex, attrTexCoords, attrNormal);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Model::Vertex)*count, vertices, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(attrVertex);
	glEnableVertexAttribArray(attrTexCoords);
	glEnableVertexAttribArray(attrNormal);

	glVertexAttribPointer(attrVertex, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Model::Vertex, pos));
	glVertexAttribPointer(attrTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Model::Vertex, texCoords));
	glVertexAttribPointer(attrNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Model::Vertex, normal));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
};

Model::~Model()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
};

void Model::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
};
