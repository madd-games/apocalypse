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

#include <Apoc/Video/Display.h>
#include <Apoc/Video/RenderHandler.h>
#include <Apoc/Video/OpenGL.h>
#include <Apoc/Entity/Texture.h>
#include <Apoc/Utils/UnicodeParser.h>
#include <iostream>
#include <SDL.h>

using namespace std;

extern RenderHandler *apocRenderHandler;

struct DisplayVertex
{
	float pos[4];
	float texCoords[2];
};

DisplayVertex apocDisplayBuffer[4] = {
	{
		{0.0, 0.0, 0.0, 1.0}, {0.0, 1.0}
	},
	{
		{0.0, 1.0, 0.0, 1.0}, {0.0, 0.0}
	},
	{
		{1.0, 1.0, 0.0, 1.0}, {1.0, 0.0}
	},
	{
		{1.0, 0.0, 0.0, 1.0}, {1.0, 1.0}
	}
};

Display::Glyph *Display::findGlyph(Glyph *font, unsigned long codepoint)
{
	while (font->codepoint != codepoint)
	{
		if (font->codepoint == 0) return NULL;
		font++;
	};

	return font;
};

Display::Display(int width, int height) : screenWidth(width), screenHeight(height)
{
	matProj = Matrix::Ortho(0.0, (float)width, 0.0, (float)height, 0.0, 1.0);
	uIsParticle = apocRenderHandler->getUniformLocation("uIsParticle");
	uDiffuseColor = apocRenderHandler->getUniformLocation("uDiffuseColor");
	uProjMatrix = apocRenderHandler->getUniformLocation("uProjectionMatrix");
	uViewMatrix = apocRenderHandler->getUniformLocation("uViewMatrix");
	uTexMatrix = apocRenderHandler->getUniformLocation("uModelMatrix");

	GLint attrVertex, attrTexCoords, attrNormal;
	apocRenderHandler->getAttrLocations(attrVertex, attrTexCoords, attrNormal);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DisplayVertex)*4, apocDisplayBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(attrVertex);
	glEnableVertexAttribArray(attrTexCoords);

	glVertexAttribPointer(attrVertex, 4, GL_FLOAT, GL_FALSE, sizeof(DisplayVertex), (void*) offsetof(DisplayVertex, pos));
	glVertexAttribPointer(attrTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(DisplayVertex), (void*) offsetof(DisplayVertex, texCoords));
};

void Display::beginDisplay()
{
	glDisable(GL_DEPTH_TEST);
	glUniformMatrix4fv(uProjMatrix, 1, GL_FALSE, &matProj[0][0]);
	glUniform1i(uIsParticle, 2);
	color(Vector(1.0, 1.0, 1.0, 1.0));
	notex();
	glBindVertexArray(vao);
};

void Display::color(Vector col)
{
	glUniform4fv(uDiffuseColor, 1, &col[0]);
};

void Display::texture(string name)
{
	Texture *tex = Texture::Get(name);
	tex->bind();
	Matrix identity = Matrix::Identity();
	glUniformMatrix4fv(uTexMatrix, 1, GL_FALSE, &identity[0][0]);
};

void Display::notex()
{
	apocRenderHandler->bindDefaultTextures();
	glActiveTexture(GL_TEXTURE0);
	Matrix identity = Matrix::Identity();
	glUniformMatrix4fv(uTexMatrix, 1, GL_FALSE, &identity[0][0]);
};

void Display::texcrop(float x, float y, float width, float height)
{
	Matrix mat = Matrix::Translate(x, y, 0.0) * Matrix::Scale(width, height, 1.0);
	glUniformMatrix4fv(uTexMatrix, 1, GL_FALSE, &mat[0][0]);
};

void Display::rect(int x, int y, int width, int height)
{
	Matrix matView = Matrix::Translate((float)x, (float)y, 0.0) * Matrix::Scale((float)width, (float)height, 1.0);
	glUniformMatrix4fv(uViewMatrix, 1, GL_FALSE, &matView[0][0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
};

void Display::text(int x, int y, Glyph *font, string str)
{
	UnicodeParser unicode(str);
	while (!unicode.end())
	{
		unsigned long codepoint = unicode.next();
		Glyph *glyph = findGlyph(font, codepoint);
		if (glyph != NULL)
		{
			texcrop((float)glyph->x/(float)glyph->fontWidth,
				1.0-((float)glyph->y/(float)glyph->fontHeight+(float)glyph->height/(float)glyph->fontHeight),
					(float)glyph->width/(float)glyph->fontWidth, (float)glyph->height/(float)glyph->fontHeight);
			rect(x, y, glyph->width, glyph->height);
			x += glyph->width;
		};
	};
};

void Display::cursor(int width, int height)
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	rect(x, y, width, height);
};
