#pragma once
#include "Engine.h"

class Game
{
public:
	enum class eState
	{
		Reset,
		Title,
		StartGame,
		StartLevel,
		Level,
		PlayerDead,
		GameOver
	};

public:
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

	bool IsQuit() { return quit; };

private:
	void Reset();
	void Title();
	void StartGame();
	void StartLevel();
	void Level();
	void PlayerDead();
	void GameOver();

	void OnAddScore(const nEngine::Event& event);

public:
	std::unique_ptr<nEngine::Engine> engine;
	std::unique_ptr<nEngine::Scene> scene;

private:
	bool quit{ false };

	SDL_Rect screen{ 0, 0 };
	eState state{ eState::Reset };
	int score{ 0 };
	float spawnTimer{0};
	float enemyTimer{0};
	float stateTimer{0};
	
};
