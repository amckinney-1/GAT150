#include "Game.h"

void Game::Initialize()
{
	int width = 800;
	int height = 600;
	screen.x = width;
	screen.y = height;

	// init
	engine = std::make_unique<Engine::Engine>();
	engine->Startup();
	engine->Get<Engine::Renderer>()->Create("WINDOW NAME", screen.x, screen.y);

	scene = std::make_unique<Engine::Scene>();
	scene->engine = engine.get();


	// filepath
	Engine::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	Engine::SetFilePath("../Resources");

	std::unique_ptr<Engine::Actor> actor = std::make_unique<Engine::Actor>(Engine::Transform{ Engine::Vector2{screen.x / 2, screen.y / 2} });
	{
		std::unique_ptr<Engine::SpriteComponent> component = std::make_unique<Engine::SpriteComponent>();
		component->texture = engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("sf2.png", engine->Get<Engine::Renderer>());
		actor->AddComponent(std::move(component));
	}
	{
		std::unique_ptr<Engine::PhysicsComponent> component = std::make_unique<Engine::PhysicsComponent>();
		component->ApplyForce(Engine::Vector2::right * 200);
		actor->AddComponent(std::move(component));
	}
	scene->AddActor(std::move(actor));

}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();
	if (engine->Get<Engine::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == Engine::InputSystem::eKeyState::Pressed) quit = true;
	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<Engine::Renderer>()->BeginFrame();
	
	scene->Draw(engine->Get<Engine::Renderer>());
	engine->Draw(engine->Get<Engine::Renderer>());

	engine->Get<Engine::Renderer>()->EndFrame();
}
