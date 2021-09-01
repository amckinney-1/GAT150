#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/PickupComponent.h"

void Game::Initialize()
{
	screen.x = 1600;
	screen.y = 1000;

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
	
	// events
	engine->Get<nEngine::EventSystem>()->Subscribe("add_score", std::bind(&Game::OnAddScore, this, std::placeholders::_1));
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

	switch (state)
	{
	case Game::eState::Reset:
		Reset();
		break;
	case Game::eState::Title:
		Title();
		break;
	case Game::eState::StartGame:
		StartGame();
		break;
	case Game::eState::StartLevel:
		StartLevel();
		break;
	case Game::eState::Level:
		Level();
		break;
	case Game::eState::PlayerDead:
		PlayerDead();
		break;
	case Game::eState::GameOver:
		GameOver();
		break;
	default:
		break;
	}

	// update score
	auto scoreActor = scene->FindActor("Score");
	if (scoreActor)
	{
		scoreActor->GetComponent<nEngine::TextComponent>()->SetText(std::to_string(score));
	}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<nEngine::Renderer>()->BeginFrame();
	
	scene->Draw(engine->Get<nEngine::Renderer>());
	engine->Draw(engine->Get<nEngine::Renderer>());

	engine->Get<nEngine::Renderer>()->EndFrame();
}

void Game::Reset()
{
	scene->RemoveAllActors();

	rapidjson::Document document;
	bool success = nEngine::json::Load("title.txt", document);
	assert(success);
	scene->Read(document);

	state = eState::Title;
}

void Game::Title()
{
	if (engine->Get<nEngine::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nEngine::InputSystem::eKeyState::Pressed)
	{
		auto title = scene->FindActor("Title");
		title->active = false;

		state = eState::StartGame;
	}
}

void Game::StartGame()
{
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

	state = eState::StartLevel;
	stateTimer = 0;
}

void Game::StartLevel()
{
	stateTimer += engine->time.deltaTime;
	if (stateTimer >= 1)
	{
		auto player = nEngine::ObjectFactory::Instance().Create<nEngine::Actor>("Player");
		player->transform.position = { 400, 300 };
		scene->AddActor(std::move(player));

		spawnTimer = 2;
		state = eState::Level;
	}
}

void Game::Level()
{
	spawnTimer -= engine->time.deltaTime;
	if (spawnTimer <= 0)
	{
		spawnTimer = nEngine::RandomRange(2, 4);

		auto coin = nEngine::ObjectFactory::Instance().Create<nEngine::Actor>("Coin");
		coin->transform.position = nEngine::Vector2{ nEngine::RandomRangeInt(100, 700), 150 };
		scene->AddActor(std::move(coin));
	}
}

void Game::PlayerDead()
{

}

void Game::GameOver()
{

}

void Game::OnAddScore(const nEngine::Event& event)
{
	score += std::get<int>(event.data);
}
