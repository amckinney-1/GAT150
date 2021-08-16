#pragma once
#include "Object/Actor.h"
#include "Graphics/Texture.h"

class Player : public Engine::Actor
{
public:
	Player(const Engine::Transform& transform, std::shared_ptr<Engine::Texture> texture, float speed) : Engine::Actor{ transform, texture }, speed{speed} {};

	void Initialize() override;

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	std::string tag{ "Player" };
	float fireTimer{ 0 };
	float fireRate{ 0.25f };
	float speed{ 1000 };
	Engine::Vector2 velocity;
};