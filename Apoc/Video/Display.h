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

#ifndef APOC_VIDEO_DISPLAY_H
#define APOC_VIDEO_DISPLAY_H

#include <Apoc/Math/Matrix.h>
#include <Apoc/Math/Vector.h>
#include <Apoc/Video/OpenGL.h>
#include <string>

using namespace std;

/**
 * \brief Handles the display of 2D overlays.
 *
 * Never make instances of this class! Only use the reference passed to onOverlay()
 * in Game.
 */
class Display
{
private:
	int screenWidth;
	int screenHeight;

	Matrix matProj;
	GLuint uIsParticle;		// we set uIsParticle to 2 in the fragment shader to draw overlays.
	GLuint uDiffuseColor;
	GLuint uProjMatrix;
	GLuint uViewMatrix;
	GLuint uTexMatrix;

	GLuint vao, vbo;

public:
	/**
	 * \brief Arrays of glyphs are used as fonts.
	 *
	 * Terminate the list of glyphs with a glyph for codepoint 0.
	 */
	struct Glyph
	{
		/**
		 * \brief The Unicode codepoint for this glyph.
		 */
		unsigned long codepoint;
		int x, y, width, height;
		int fontWidth, fontHeight;
	};

private:
	Glyph* findGlyph(Glyph *font, unsigned long codepoint);
public:

	/**
	 * \brief Constructor. Do not use.
	 */
	Display(int width, int height);

	/**
	 * \brief Do not use.
	 */
	void beginDisplay();

	/**
	 * \brief Set the color to draw things with.
	 */
	void color(Vector col);

	/**
	 * \brief Set the texture to draw stuff with.
	 */
	void texture(string name);

	/**
	 * \brief Set the texture to "plain".
	 */
	void notex();

	/**
	 * \brief Sets which part of the texture to apply.
	 *
	 * The coordinates and size are normalized.
	 */
	void texcrop(float x, float y, float width, float height);

	/**
	 * \brief Draw a rectangle.
	 */
	void rect(int x, int y, int width, int height);

	/**
	 * \brief Draw text.
	 * \param font An array of Glyph structures, terminated with a glyph for codepoint 0.
	 * \param str UTF-8 formatted string.
	 */
	void text(int x, int y, Glyph *font, string str);

	/**
	 * \brief Draw the mouse cursor using the current texture.
	 * \param width Width of the cursor.
	 * \param height Height of the cursor.
	 */
	void cursor(int width, int height);
};

#endif
