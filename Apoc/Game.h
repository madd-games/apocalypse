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

#ifndef APOC_GAME_H
#define APOC_GAME_H

#include <Apoc/Video/OpenGL.h>

/**
 * \brief Base for main game classes.
 *
 * Each game must declare a class deriving from this one, called `GameImpl`, in `Game/GameImpl.h`.
 */
class Game
{
public:
	/**
	 * \brief Called when the game is started.
	 */
	virtual void onGameStart();

	/**
	 * \brief Called when a key is pressed.
	 * \param key The key pressed (see SDL documentation).
	 */
	virtual void onKeyPress(SDL_Keycode key);

	/**
	 * \brief Called when a key is released.
	 * \param key The key released (see SDL documentation).
	 */
	virtual void onKeyRelease(SDL_Keycode key);

	/**
	 * \brief Called before the world update.
	 */
	virtual void update();

	/**
	 * \brief Called when the mouse moves.
	 *
	 * Use SDL_GetMouseState() to get the mouse position and ApocWarpMouse() from Apoc/Utils/Utils.h to move the mouse.
	 * See SDL documentation.
	 */
	virtual void onMouseMove();

	/**
	 * \brief Specifies the desired screen resolution (or window size, depending on what isFullscreen returns).
	 */
	virtual void getScreenSize(int &width, int &height);

	/**
	 * \brief Returns true if a fullscreen mode is desired.
	 */
	virtual bool isFullscreen();
};

#endif
