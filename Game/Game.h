#pragma once
#include "Engine.h"

class Game
{
public:
	enum class eState
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		GameOver
	};

public:
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

	bool IsQuit() { return quit; };

private:
	void UpdateLevelStart(float dt);

	void OnAddPoints(const Engine::Event& event);
	void OnPlayerDead(const Engine::Event& event);

public:
	std::unique_ptr<Engine::Engine> engine;
	std::unique_ptr<Engine::Scene> scene;

private:
	SDL_Rect screen{ 0, 0 };

	bool quit{ false };
	eState state{ eState::Title };
	float stateTimer{ 0.0f };
	float levelTimer{ 0.0f };

	size_t score{ 0 };
	size_t lives{ 0 };
	size_t level{ 0 };

	Engine::AudioChannel musicChannel;
	std::shared_ptr<Engine::Texture> particleTexture;
	std::shared_ptr<Engine::Texture> textTexture;
};