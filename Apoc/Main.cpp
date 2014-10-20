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

#include <Apoc/Video/OpenGL.h>
#include <Apoc/Utils/Utils.h>
#include <Apoc/Video/StandardRenderHandler.h>
#include <Apoc/Entity/Model.h>
#include <Apoc/Math/Matrix.h>
#include <Apoc/Entity/Texture.h>
#include <Apoc/Entity/Entity.h>
#include <Apoc/Entity/World.h>
#include <Apoc/Physics/CollisionCheck.h>

#include <Game/GameImpl.h>

#ifdef ENABLE_OPENCL
#include <Apoc/Compute/Compute.h>
#endif

#include <math.h>
#include <string>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

using namespace std;

SDL_Window *apocWindow;
SDL_GLContext apocContext;
RenderHandler *apocRenderHandler;

void ApocMoveMouse(int x, int y)
{
	SDL_WarpMouseInWindow(apocWindow, x, y);
};

float apocRenderTime;		// used by ApocGetRenderTime() and ApocGetFPS() in Apoc/Utils/Utils.cpp

int main(int argc, char *argv[])
{
#ifdef CLIENT
#ifdef __GNUC__
	cout << "[APOC] GCC Vector Extensions are enabled" << endl;
#endif
	Game *game = new GameImpl;

	bool allowCL = true;
	int i;
	for (i=0; i<argc; i++)
	{
		string str = argv[i];
		if (str == "--no-cl")
		{
			allowCL = false;
		};
	};

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		ApocFail("Cannot initialize SDL!");
	};

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	int screenWidth, screenHeight;
	game->getScreenSize(screenWidth, screenHeight);

	int flagsFullscreen = game->isFullscreen() ? SDL_WINDOW_FULLSCREEN:0;

	apocWindow = SDL_CreateWindow("Apocalypse", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			screenWidth, screenHeight, SDL_WINDOW_OPENGL | flagsFullscreen);
	if (apocWindow == NULL)
	{
		ApocFail(string("SDL_CreateWindow failed: ") + SDL_GetError());
	};

	SDL_ShowCursor(0);

	apocContext = SDL_GL_CreateContext(apocWindow);
	if (apocContext == NULL)
	{
		ApocFail(string("Could not create GL context: ") + SDL_GetError());
	};

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		ApocFail("Cannot initialise GLEW!");
	};

	while (glGetError() != GL_NO_ERROR);	// clearing the error flag first?

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	apocRenderHandler = new StandardRenderHandler(screenWidth, screenHeight);
	apocRenderHandler->bindProgram();

#ifdef ENABLE_OPENCL
	if (allowCL)
	{
		InitCompute();
	}
	else
	{
		cout << "[APOC] [COMPUTE] OpenCL disabled in command line!" << endl;
	};
#else
	cout << "[APOC] [COMPUTE] OpenCL support disabled at compile-time!" << endl;
#endif

	cout << "[APOC] Loading textures" << endl;
	Texture::Init();

	GLint uModelMatrix = apocRenderHandler->getUniformLocation("uModelMatrix");
	GLint uViewMatrix = apocRenderHandler->getUniformLocation("uViewMatrix");
	GLint uProjectionMatrix = apocRenderHandler->getUniformLocation("uProjectionMatrix");

	Matrix matModel = Matrix::Identity();
	Matrix matView = Matrix::LookAt(
		Vector(0.0, 2.0, -10.0, 1.0),
		Vector(0.0, 1.0, 0.0, 0.0),
		Vector(0.0, 0.0, 0.0, 1.0)
	);
	Matrix matProj = Matrix::Perspective(1366.0, 768.0, 1.0, 100.0, 60.0*M_PI/180.0);

	glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, &matModel[0][0]);
	glUniformMatrix4fv(uViewMatrix, 1, GL_FALSE, &matView[0][0]);
	glUniformMatrix4fv(uProjectionMatrix, 1, GL_FALSE, &matProj[0][0]);

	SDL_Event event;
	bool quit = false;
	unsigned long lastTicks;
	game->onGameStart();
	while (!quit)
	{
		lastTicks = SDL_GetTicks();
		
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			};
			
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
			else if (event.type == SDL_KEYUP)
			{
				SDL_Keycode key = event.key.keysym.sym;
				game->onKeyRelease(key);
			};

			if (event.type == SDL_KEYDOWN)
			{
				SDL_Keycode key = event.key.keysym.sym;
				game->onKeyPress(key);
			};

			if (event.type == SDL_MOUSEMOTION)
			{
				game->onMouseMove();
			};
		};

		game->update();
		World::update();
		apocRenderHandler->render();

		// Measure the time needed to render this frame.
		apocRenderTime = (float)(SDL_GetTicks() - lastTicks) / 1000.0;
		
		glFlush();
		SDL_GL_SwapWindow(apocWindow);
	};

#ifdef ENABLE_OPENCL
	QuitCompute();
#endif

	SDL_GL_DeleteContext(apocContext);
	SDL_DestroyWindow(apocWindow);
	SDL_Quit();
#endif
	return 0;
};
