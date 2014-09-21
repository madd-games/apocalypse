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

#include <math.h>
#include <string>
#include <iostream>

using namespace std;

SDL_Window *apocWindow;
SDL_GLContext apocContext;
RenderHandler *apocRenderHandler;

extern "C" const Model::Vertex modVertices_test[];
extern "C" const int modCount_test;

int main()
{
#ifdef CLIENT
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		ApocFail("Cannot initialize SDL!");
	};

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	apocWindow = SDL_CreateWindow("Apocalypse", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			800, 600, SDL_WINDOW_OPENGL);
			
	apocContext = SDL_GL_CreateContext(apocWindow);
	
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		ApocFail("Cannot initialise GLEW!");
	};
	
	apocRenderHandler = new StandardRenderHandler();
	apocRenderHandler->bindProgram();

	Model *model = new Model(modVertices_test, modCount_test);

	GLint uModelMatrix = apocRenderHandler->getUniformLocation("uModelMatrix");
	GLint uViewMatrix = apocRenderHandler->getUniformLocation("uViewMatrix");
	GLint uProjectionMatrix = apocRenderHandler->getUniformLocation("uProjectionMatrix");

	Matrix matModel = Matrix::Identity();
	Matrix matView = Matrix::LookAt(
		Vector(0.0, 0.0, -10.0, 1.0),
		Vector(0.0, 1.0, 0.0, 0.0),
		Vector(0.0, 0.0, 0.0, 1.0)
	);
	Matrix matProj = Matrix::Perspective(800.0, 600.0, 1.0, 1000.0, 60.0*M_PI/180.0);

	glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, &matModel[0][0]);
	glUniformMatrix4fv(uViewMatrix, 1, GL_FALSE, &matView[0][0]);
	glUniformMatrix4fv(uProjectionMatrix, 1, GL_FALSE, &matProj[0][0]);

	SDL_Event event;
	bool quit = false;
	while (!quit)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			};
			
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			};
		};
		
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//apocRenderHandler->render(NULL);

		model->draw();
		glFlush();
		SDL_GL_SwapWindow(apocWindow);
	};

	delete model;
	SDL_GL_DeleteContext(apocContext);
	SDL_Quit();
#endif
	return 0;
};
