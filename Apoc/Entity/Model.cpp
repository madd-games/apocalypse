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
#include <Apoc/Math/Vector.h>

Model::Model(Model::Vertex *vertices, const int count) : vertexCount(count), data(vertices)
{
	if (count == 0)
	{
		ApocFail("Empty model specified!");
	};

	if ((count % 3) != 0)
	{
		ApocFail("The specified model is not triangulated!");
	};

	minVector = vertices[0].pos;
	maxVector = vertices[0].pos;

	int i;
	for (i=1; i<count; i++)
	{
		Vector pos = vertices[i].pos;

		int j;
		for (j=0; j<3; j++)
		{
			if (pos[j] < minVector[j]) minVector[j] = pos[j];
			if (pos[j] > maxVector[j]) maxVector[j] = pos[j];
		};
	};

	// compute the tangents for each triangle.
	int i;
	for (i=0; i<count; i+=3)
	{
		Vector &v0 = vertices[i+0].pos;
		Vector &v1 = vertices[i+1].pos;
		Vector &v2 = vertices[i+2].pos;
		
		Vector &uv0 = vertices[i+0].texCoords;
		Vector &uv1 = vertices[i+1].texCoords;
		Vector &uv2 = vertices[i+2].texCoords;
		
		Vector deltaPos1 = v1 - v0;
		Vector deltaPos2 = v2 - v0;
		
		Vector deltaUV1 = uv1 - uv0;
		Vector deltaUV2 = uv2 - uv0;
		
		float r = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV1.y() * deltaUV2.x());
		Vector vtan = (deltaPos2 * deltaUV1.x() - deltaPos1 * deltaUV2.x())*r;
		Vector utan = (deltaPos1 * deltaUV2.y() - deltaPos2 * deltaUV1.y())*r;
		
		int j;
		for (j=0; j<3; j++)
		{
			vertices[i+j].vtan = vtan;
			vertices[i+j].utan = utan;
		};
	};
	
	GLint attrVertex, attrTexCoords, attrNormal, attrVTan, attrUTan;
	apocRenderHandler->getAttrLocations(attrVertex, attrTexCoords, attrNormal);
	attrVTan = apocRenderHandler->getAttrLocation("inVTan");
	attrUTan = apocRenderHandler->getAttrLocation("inUTan");
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Model::Vertex)*count, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(attrVertex);
	glEnableVertexAttribArray(attrTexCoords);
	glEnableVertexAttribArray(attrNormal);
	glEnableVertexAttribArray(attrVTan);
	glEnableVertexAttribArray(attrUTan);
	
	glVertexAttribPointer(attrVertex, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Model::Vertex, pos));
	glVertexAttribPointer(attrTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Model::Vertex, texCoords));
	glVertexAttribPointer(attrNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Model::Vertex, normal));
	glVertexAttribPointer(attrVTan, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Model::Vertex, vtan));
	glVertexAttribPointer(attrUTan, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Model::Vertex, utan));
};

Model::~Model()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
};

void Model::draw()
{
	glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
};
