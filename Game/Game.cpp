#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"

void Game::Initialize()
{
	screen.x = 800;
	screen.y = 600;

	// init
	engine = std::make_unique<nEngine::Engine>();
	engine->Startup();
	engine->Get<nEngine::Renderer>()->Create("WINDOW NAME", screen.x, screen.y);

	// register classes
	REGISTER_CLASS(PlayerComponent);
	REGISTER_CLASS(EnemyComponent);


	scene = std::make_unique<nEngine::Scene>();
	scene->engine = engine.get();


	// filepath
	nEngine::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nEngine::SetFilePath("../Resources");

	rapidjson::Document document;
	bool success = nEngine::json::Load("scene.txt", document);
	assert(success);
	scene->Read(document);



	//std::unique_ptr<Engine::Actor> actor = std::make_unique<Engine::Actor>(Engine::Transform{ Engine::Vector2{screen.x / 2, screen.y / 2}, 0 , 2 });
	//{
	//	auto component = Engine::ObjectFactory::Instance().Create<Engine::SpriteAnimationComponent>("SpriteAnimationComponent");

	//	component->texture = engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("Sprites/character.png", engine->Get<Engine::Renderer>());
	//	component->fps = 30;
	//	component->numFramesX = 12;
	//	component->numFramesY = 8;
	//	
	//	actor->AddComponent(std::move(component));
	//}
	////{
	//	//Engine::PhysicsComponent* component = actor->AddComponent<Engine::PhysicsComponent>();
	//	//component->ApplyForce(Engine::Vector2::right * 200);

	////}
	//scene->AddActor(std::move(actor));

}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();
	if (engine->Get<nEngine::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nEngine::InputSystem::eKeyState::Pressed) quit = true;
	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<nEngine::Renderer>()->BeginFrame();
	
	scene->Draw(engine->Get<nEngine::Renderer>());
	engine->Draw(engine->Get<nEngine::Renderer>());

	engine->Get<nEngine::Renderer>()->EndFrame();
}
