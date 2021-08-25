#include "Game.h"
#include <fstream>
#include <cassert>


int main(int, char**)
{
	Game game;
	game.Initialize();
	/*
	rapidjson::Document document;
	bool success = Engine::json::Load("json.txt", document);
	assert(success);

	int i1;
	Engine::json::Get(document, "json.txt", i1);
	std::cout << i1 << std::endl;

	int i2;
	Engine::json::Get(document, "json.txt", i2);
	std::cout << i2 << std::endl;
	*/





	// create engine
	Engine::Engine engine;
	engine.Startup();

	// create scene
	Engine::Scene scene;
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