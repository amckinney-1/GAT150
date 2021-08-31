#include "Game.h"
#include <fstream>
#include <cassert>


int main(int, char**)
{
	Game game;
	game.Initialize();

	// create engine
	nEngine::Engine engine;
	engine.Startup();

	// create scene
	nEngine::Scene scene;
	scene.engine = &engine;

	bool quit = false;
	SDL_Event event;

	while (!quit && !game.IsQuit())
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		game.Update();
		game.Draw();
	}

	SDL_Quit();

	return 0;
}