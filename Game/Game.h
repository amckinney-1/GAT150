#pragma once
#include "Engine.h"

class Game
{

public:
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

	bool IsQuit() { return quit; };

private:
	
public:
	std::unique_ptr<nEngine::Engine> engine;
	std::unique_ptr<nEngine::Scene> scene;

private:
	bool quit{ false };

	SDL_Rect screen{ 0, 0 };
	
};
