#include "Engine.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>

int main(int, char**)
{
	Engine::Engine engine;
	engine.Startup();
	engine.Get<Engine::Renderer>()->Create("WINDOW NAME", 800, 600);

	Engine::Scene scene;
	scene.engine = &engine;

	Engine::SetFilePath("../Resources");

	std::shared_ptr<Engine::Texture> texture = engine.Get<Engine::ResourceSystem>()->Get<Engine::Texture>("sf2.png", engine.Get<Engine::Renderer>());

	for (size_t i = 0; i < 200; i++)
	{
		Engine::Transform transform{ { Engine::RandomRange(0, 800),  Engine::RandomRange(0, 600)}, Engine::RandomRange(0, 360), 1.0f };
		std::unique_ptr<Engine::Actor> actor = std::make_unique<Engine::Actor>(transform, texture);
		scene.AddActor(std::move(actor));
	}

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		engine.Update(0);
		scene.Update(0);

		engine.Get<Engine::Renderer>()->BeginFrame();

		scene.Draw(engine.Get<Engine::Renderer>());

		engine.Get<Engine::Renderer>()->EndFrame();
	}

	
	SDL_Quit();

	return 0;
}