#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/PickupComponent.h"

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
	REGISTER_CLASS(PickupComponent);


	scene = std::make_unique<nEngine::Scene>();
	scene->engine = engine.get();


	// filepath
	nEngine::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nEngine::SetFilePath("../Resources");

	rapidjson::Document document;
	bool success = nEngine::json::Load("scene.txt", document);
	assert(success);

	scene->Read(document);

	nEngine::Tilemap tilemap;
	tilemap.scene = scene.get();
	success = nEngine::json::Load("map.txt", document);
	assert(success);
	tilemap.Read(document);
	tilemap.Create();

	for (int i = 0; i < 10; i++)
	{
		auto actor = nEngine::ObjectFactory::Instance().Create<nEngine::Actor>("Coin");
		actor->transform.position = nEngine::Vector2{ nEngine::RandomRange(0, 800), nEngine::RandomRange(300, 450) };
		scene->AddActor(std::move(actor));
	}
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
