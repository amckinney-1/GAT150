#include "Game.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"

void Game::Initialize()
{
	int width = 800;
	int height = 600;
	screen.x = width;
	screen.y = height;

	// init engine/scene
	engine = std::make_unique<Engine::Engine>();
	engine->Startup();

	scene = std::make_unique<Engine::Scene>();
	scene->engine = engine.get();

	// create window
	engine->Get<Engine::Renderer>()->Create("WINDOW NAME", screen.x, screen.y);

	// filepath
	Engine::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	Engine::SetFilePath("../Resources");

	// get font from resource system
	int size = 16;
	std::shared_ptr<Engine::Font> font = engine->Get<Engine::ResourceSystem>()->Get<Engine::Font>("fonts/ALGER.TTF", &size);

	// create font texture
	textTexture = std::make_shared<Engine::Texture>(engine->Get<Engine::Renderer>());
	// set font texture with font surface
	textTexture->Create(font->CreateSurface("hello world", Engine::Color{ 1, 1, 1 }));
	// add font texture to resource system
	engine->Get<Engine::ResourceSystem>()->Add("textTexture", textTexture);

	engine->Get<Engine::AudioSystem>()->AddAudio("music", "audio/music.mp3");
	musicChannel = engine->Get<Engine::AudioSystem>()->PlayAudio("music", 1, 1.5f, true);

	// game stuff
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
			scene->engine->Get<Engine::AudioSystem>()->PlayAudio("music");
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
	if (engine->Get<Engine::InputSystem>()->GetButtonState((int)Engine::InputSystem::eMouseButton::Left) == Engine::InputSystem::eKeyState::Held)
	{
		Engine::Vector2 position = engine->Get<Engine::InputSystem>()->GetMousePosition();
		// create particles
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
		/*graphics.SetColor(Engine::Color::purple);

		graphics.DrawString(320, 300 + static_cast<int>(std::sin(stateTimer * 10) * 20), "[Insert Game Title Here]");
		graphics.DrawString(320, 400, "Press Space to Start");*/
		break;
	case Game::eState::StartGame:
		
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
		break;
	case Game::eState::GameOver:
		/*graphics.SetColor(Engine::Color::red);
		graphics.DrawString(320, 400, "GAME OVER");*/
		break;
	default:
		break;
	}
	/*graphics.SetColor(Engine::Color::white);
	graphics.DrawString(30, 20, std::to_string(score).c_str());
	graphics.DrawString(400, 20, std::to_string(level).c_str());
	graphics.DrawString(770, 20, std::to_string(lives).c_str());*/

	scene->Draw(engine->Get<Engine::Renderer>());
	engine->Draw(engine->Get<Engine::Renderer>());

	// draw

	Engine::Transform t;
	t.position = { screen.x / 2, screen.y / 2 };
	engine->Get<Engine::Renderer>()->Draw(textTexture, t);

	engine->Get<Engine::Renderer>()->EndFrame();
}

void Game::UpdateLevelStart(float dt)
{
	if (!scene->GetActor<Player>())
	{
		scene->AddActor(std::make_unique<Player>(Engine::Transform{ Engine::Vector2{screen.x / 2, screen.y / 2}, 0, 0.5f }, engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("spaceShips_004.png", engine->Get<Engine::Renderer>()), 300.0f));
	}
	/*
	for (size_t i = 0; i < level + 2; i++)
	{
		float f = Engine::RandomRange(0, 10);
		if (f <= 7.6)
		{
			scene->AddActor(std::make_unique<Enemy>(Engine::Transform{ Engine::Vector2{Engine::RandomRange(0.0f, screen.x),Engine::RandomRange(0.0f, screen.y)}, Engine::RandomRange(0, Engine::TwoPi), 0.5f }, engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("spaceShips_003.png", engine->Get<Engine::Renderer>()), 150.0f));
		}
		else if (f <= 9)
		{
			scene->AddActor(std::make_unique<Enemy>(Engine::Transform{ Engine::Vector2{Engine::RandomRange(0.0f, screen.x),Engine::RandomRange(0.0f, screen.y)}, Engine::RandomRange(0, Engine::TwoPi), 0.5f }, engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("spaceShips_002.png", engine->Get<Engine::Renderer>()), 100.0f, true));
		}
		else
		{
			scene->AddActor(std::make_unique<Enemy>(Engine::Transform{ Engine::Vector2{Engine::RandomRange(0.0f, screen.x),Engine::RandomRange(0.0f, screen.y)}, Engine::RandomRange(0, Engine::TwoPi), 0.5f }, engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("spaceShips_001.png", engine->Get<Engine::Renderer>()), 50.0f, true, true));
		}

	}
	*/
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
