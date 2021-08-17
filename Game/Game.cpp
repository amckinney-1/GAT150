#include "Game.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"

void Game::Initialize()
{
	int width = 800;
	int height = 600;
	screen.x = width;
	screen.y = height;

	// init
	engine = std::make_unique<Engine::Engine>();
	engine->Startup();

	scene = std::make_unique<Engine::Scene>();
	scene->engine = engine.get();

	engine->Get<Engine::Renderer>()->Create("WINDOW NAME", screen.x, screen.y);

	// filepath
	Engine::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	Engine::SetFilePath("../Resources");

	// Screen Text
	int size = 24;
	std::shared_ptr<Engine::Font> font = engine->Get<Engine::ResourceSystem>()->Get<Engine::Font>("fonts/ALGER.TTF", &size);

	std::shared_ptr<Engine::Texture> titleTexture = std::make_shared<Engine::Texture>(engine->Get<Engine::Renderer>());
	titleTexture->Create(font->CreateSurface("Spacetroids", Engine::Color::white));
	engine->Get<Engine::ResourceSystem>()->Add("titleTexture", titleTexture);

	std::shared_ptr<Engine::Texture> gameoverTexture = std::make_shared<Engine::Texture>(engine->Get<Engine::Renderer>());
	gameoverTexture->Create(font->CreateSurface("GAME  OVER", Engine::Color::white));
	engine->Get<Engine::ResourceSystem>()->Add("gameoverTexture", gameoverTexture);

	std::shared_ptr<Engine::Texture> scoreTexture = std::make_shared<Engine::Texture>(engine->Get<Engine::Renderer>());
	scoreTexture->Create(font->CreateSurface("0000", Engine::Color::white));
	engine->Get<Engine::ResourceSystem>()->Add("scoreTexture", scoreTexture);

	std::shared_ptr<Engine::Texture> levelTexture = std::make_shared<Engine::Texture>(engine->Get<Engine::Renderer>());
	scoreTexture->Create(font->CreateSurface("00", Engine::Color::white));
	engine->Get<Engine::ResourceSystem>()->Add("levelTexture", levelTexture);

	std::shared_ptr<Engine::Texture> lifeTexture = std::make_shared<Engine::Texture>(engine->Get<Engine::Renderer>());
	scoreTexture->Create(font->CreateSurface("0", Engine::Color::white));
	engine->Get<Engine::ResourceSystem>()->Add("lifeTexture", lifeTexture);

	// Game Audio
	engine->Get<Engine::AudioSystem>()->AddAudio("music", "audio/music.mp3");
	musicChannel = engine->Get<Engine::AudioSystem>()->PlayAudio("music", 1, 1.05f, true);

	engine->Get<Engine::AudioSystem>()->AddAudio("explosion", "explosion.wav");
	engine->Get<Engine::AudioSystem>()->AddAudio("PlayerShoot", "PlayerShoot.wav");
	engine->Get<Engine::AudioSystem>()->AddAudio("EnemyShoot", "EnemyShoot.wav");
	engine->Get<Engine::AudioSystem>()->AddAudio("PlayerExplosion", "PlayerExplosion.wav");
	engine->Get<Engine::AudioSystem>()->AddAudio("EnemyExplosion", "EnemyExplosion.wav");
	engine->Get<Engine::AudioSystem>()->AddAudio("music", "music.mp3");
	
	engine->Get<Engine::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1));
	engine->Get<Engine::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1));
}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();
	stateTimer += engine->time.deltaTime;

	switch (state)
	{
	case Game::eState::Title:
		if (engine->Get<Engine::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == Engine::InputSystem::eKeyState::Pressed)
		{
			//scene->engine->Get<Engine::AudioSystem>()->PlayAudio("music");
			state = eState::StartGame;
		}
		break;
	case Game::eState::StartGame:
		score = 0;
		lives = 3;
		state = eState::StartLevel;
		break;
	case Game::eState::StartLevel:
		levelTimer = 0;
		level++;
		UpdateLevelStart(engine->time.deltaTime);
		state = eState::Game;
		break;
	case Game::eState::Game:
		levelTimer += engine->time.deltaTime;
		if (levelTimer >= 5)
		{
			levelTimer -= 5;
			int spawnNumber = Engine::RandomRangeInt(1 + level, 3 + level);

			for (int i = 0; i <= spawnNumber; i++)
			{
				//scene->AddActor(std::make_unique<Asteroid>(Engine::Transform{ Engine::Vector2{Engine::RandomRange(0.0f, 800.0f), 0.0f}, Engine::RandomRange(0, Engine::TwoPi), Engine::RandomRange(1, 7) }, engine->Get<Engine::ResourceSystem>()->Get<Engine::Shape>("Asteroid.txt"), 300.0f));
			}
		}
		if (lives > 0 && !scene->GetActor<Player>())
		{
			lives--;
			scene->AddActor(std::make_unique<Player>(Engine::Transform{ Engine::Vector2{screen.x / 2, screen.y / 2}, 0, 0.5f }, engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("spaceShips_004.png", engine->Get<Engine::Renderer>()), 300.0f));
		}
		if (lives <= 0 && !scene->GetActor<Player>())
		{
			state = eState::GameOver;
		}
		else if (scene->GetActors<Enemy>().size() == 0)
		{
			state = eState::StartLevel;
		}
		break;
	case Game::eState::GameOver:
		break;
	default:
		break;
	}

	if (engine->Get<Engine::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == Engine::InputSystem::eKeyState::Pressed) quit = true;


	// update
	if (engine->Get<Engine::InputSystem>()->GetButtonState((int)Engine::InputSystem::eMouseButton::Left) == Engine::InputSystem::eKeyState::Pressed)
	{
		Engine::Vector2 position = engine->Get<Engine::InputSystem>()->GetMousePosition();
		// create particles
		engine->Get<Engine::ParticleSystem>()->Create(position, 20, 1, engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("particle01.png", engine->Get<Engine::Renderer>()), 100);
		musicChannel.SetPitch(Engine::RandomRange(0.2f, 2.0f));
	}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<Engine::Renderer>()->BeginFrame();
	switch (state)
	{
	case Game::eState::Title:
	{
		Engine::Transform t;
		t.position = { screen.x / 2, screen.y / 2 };
		engine->Get<Engine::Renderer>()->Draw(engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("titleTexture"), t);
	}

	break;
	case Game::eState::StartGame:

		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
		break;
	case Game::eState::GameOver:
	{
		Engine::Transform t;
		t.position = { screen.x / 2, screen.y / 2 };
		engine->Get<Engine::Renderer>()->Draw(engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("gameoverTexture"), t);
	}
	break;
	default:
		break;
	}

	// score text
	std::shared_ptr<Engine::Font> font = engine->Get<Engine::ResourceSystem>()->Get<Engine::Font>("fonts/ALGER.TTF");

	Engine::Transform t;
	t.position = { 50, 20 };
	std::shared_ptr<Engine::Texture> scoreTexture = engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("scoreTexture");
	scoreTexture->Create(font->CreateSurface(std::to_string(score), Engine::Color::white));
	engine->Get<Engine::Renderer>()->Draw(scoreTexture, t);

	t.position.x = screen.x / 2;
	t.position.y = 20.0f;
	std::shared_ptr<Engine::Texture> levelTexture = engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("levelTexture");
	levelTexture->Create(font->CreateSurface(std::to_string(level), Engine::Color::white));
	engine->Get<Engine::Renderer>()->Draw(levelTexture, t);

	t.position.x = screen.x - 50.0f;
	t.position.y = 20.0f;
	std::shared_ptr<Engine::Texture> lifeTexture = engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("lifeTexture");
	lifeTexture->Create(font->CreateSurface(std::to_string(lives), Engine::Color::white));
	engine->Get<Engine::Renderer>()->Draw(lifeTexture, t);

	scene->Draw(engine->Get<Engine::Renderer>());
	engine->Draw(engine->Get<Engine::Renderer>());

	engine->Get<Engine::Renderer>()->EndFrame();
}

void Game::UpdateLevelStart(float dt)
{
	if (!scene->GetActor<Player>())
	{
		scene->AddActor(std::make_unique<Player>(Engine::Transform{ Engine::Vector2{screen.x / 2, screen.y / 2}, 0, 0.5f }, engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("spaceShips_004.png", engine->Get<Engine::Renderer>()), 300.0f));
	}
	
	for (size_t i = 0; i < level + 2; i++)
	{
		Engine::Vector2 position;
		if (Engine::RandomRangeInt(0, 2) == 0)
		{
			position.x = (Engine::RandomRangeInt(0, 2) == 0) ? 0.0f : screen.x;
			position.y = Engine::RandomRange(0, screen.y);
		}
		else
		{
			position.x = Engine::RandomRange(0, screen.x);
			position.y = (Engine::RandomRangeInt(0, 2) == 0) ? 0.0f : screen.y;
		}

		float f = Engine::RandomRange(0, 10);
		if (f <= 7.6)
		{
			scene->AddActor(std::make_unique<Enemy>(Engine::Transform{ position, Engine::RandomRange(0, Engine::TwoPi), 0.5f }, engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("spaceShips_003.png", engine->Get<Engine::Renderer>()), 150.0f));
		}
		else if (f <= 9)
		{
			scene->AddActor(std::make_unique<Enemy>(Engine::Transform{ position, Engine::RandomRange(0, Engine::TwoPi), 0.5f }, engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("spaceShips_002.png", engine->Get<Engine::Renderer>()), 100.0f, true));
		}
		else
		{
			scene->AddActor(std::make_unique<Enemy>(Engine::Transform{ position, Engine::RandomRange(0, Engine::TwoPi), 0.5f }, engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("spaceShips_001.png", engine->Get<Engine::Renderer>()), 50.0f, true, true));
		}

	}
	
}

void Game::OnAddPoints(const Engine::Event& event)
{
	score += std::get<int>(event.data);
}

void Game::OnPlayerDead(const Engine::Event& event)
{
	
	if (lives <= 0)
	{
		std::cout << std::get<std::string>(event.data) << std::endl;
		state = eState::GameOver;
	}

}
