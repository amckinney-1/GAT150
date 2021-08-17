#pragma once
#include "Object/Actor.h"

class Asteroid : public Engine::Actor
{
public:
	Asteroid(const Engine::Transform& transform, std::shared_ptr<Engine::Texture> texture, float speed) : Engine::Actor{ transform, texture }, speed{ speed } {};

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;
private:
	float speed{ 200 };
};