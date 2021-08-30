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

	std::string str;
	Engine::json::Get(document, "string", str);
	std::cout << str << std::endl;

	bool b;
	Engine::json::Get(document, "boolean", b);
	std::cout << b << std::endl;

	int i1;
	Engine::json::Get(document, "integer1", i1);
	std::cout << i1 << std::endl;

	int i2;
	Engine::json::Get(document, "integer2", i2);
	std::cout << i2 << std::endl;

	float f;
	Engine::json::Get(document, "float", f);
	std::cout << f << std::endl;

	Engine::Vector2 v2;
	Engine::json::Get(document, "vector2", v2);
	std::cout << v2 << std::endl;

	Engine::Color color;
	Engine::json::Get(document, "color", color);
	std::cout << color << std::endl;
	*/

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