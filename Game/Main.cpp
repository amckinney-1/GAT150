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

	engine.Get<Engine::AudioSystem>()->AddAudio("explosion", "audio/shoot.wav" );
	engine.Get<Engine::AudioSystem>()->AddAudio("music", "audio/music.mp3" );
	Engine::AudioChannel channel = engine.Get<Engine::AudioSystem>()->PlayAudio("music", 1, 1.5f, true);

	std::shared_ptr<Engine::Texture> texture = engine.Get<Engine::ResourceSystem>()->Get<Engine::Texture>("sf2.png", engine.Get<Engine::Renderer>());

	for (size_t i = 0; i < 10; i++)
	{
		Engine::Transform transform{ { Engine::RandomRange(0, 800),  Engine::RandomRange(0, 600)}, Engine::RandomRange(0, 360), 1.0f };
		std::unique_ptr<Engine::Actor> actor = std::make_unique<Engine::Actor>(transform, texture);
		scene.AddActor(std::move(actor));
	}

	bool quit = false;
	SDL_Event event;
	float quitTime = engine.time.time + 3.0f;

	while (!quit)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		// update
		engine.Update();
		scene.Update(engine.time.deltaTime);

		//std::cout << engine.time.time << std::endl;
		if (engine.time.time >= quitTime) quit = true;

		if (engine.Get<Engine::InputSystem>()->GetButtonState((int)Engine::InputSystem::eMouseButton::Left) == Engine::InputSystem::eKeyState::Held)
		{
			Engine::Vector2 position = engine.Get<Engine::InputSystem>()->GetMousePosition();
			// create particles
			
			// std::cout << position.x << " " << position.y << std::endl;
			//engine.Get<Engine::AudioSystem>()->PlayAudio("explosion", 1, Engine::RandomRange(0.2f, 2.0f));
			channel.SetPitch(Engine::RandomRange(0.2f, 2.0f));
		}

		// draw
		quit = (engine.Get<Engine::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == Engine::InputSystem::eKeyState::Pressed);
		engine.Get<Engine::Renderer>()->BeginFrame();

		scene.Draw(engine.Get<Engine::Renderer>());
		engine.Draw(engine.Get<Engine::Renderer>());

		engine.Get<Engine::Renderer>()->EndFrame();
	}
	SDL_Quit();

	return 0;
}