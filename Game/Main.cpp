#include "Engine.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>

int main(int, char**)
{
	Engine::Engine engine;
	engine.Startup();

	engine.Get<Engine::Renderer>()->Create("WINDOW NAME", 800, 600);

	std::cout << Engine::GetFilePath() << std::endl;
	Engine::SetFilePath("../Resources");
	std::cout << Engine::GetFilePath() << std::endl;

	std::shared_ptr<Engine::Texture> texture = engine.Get<Engine::ResourceSystem>()->Get<Engine::Texture>("sf2.png", engine.Get<Engine::Renderer>());

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

		engine.Get<Engine::Renderer>()->BeginFrame();

		Engine::Vector2 position{ 300, 400 };
		engine.Get<Engine::Renderer>()->Draw(texture, position);

		engine.Get<Engine::Renderer>()->EndFrame();
		
		//for (size_t i = 0; i < 200; i++)
		//{
		//	SDL_Rect src{ 32,64,32,64 };
		//	SDL_Rect dest{ Engine::RandomRangeInt(0, screen.x), Engine::RandomRangeInt(0, screen.y), 32, 48 };

		//	SDL_RenderCopy(renderer, texture, &src, &dest);
		//}
	}

	
	SDL_Quit();

	return 0;
}