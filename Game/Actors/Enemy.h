#pragma once
#include "Object/Actor.h"

class Enemy : public Engine::Actor
{
public:
	Enemy(const Engine::Transform& transform, std::shared_ptr<Engine::Texture> texture, float speed, bool isShooter = false, bool isBurst = false) : Engine::Actor{ transform, texture }, speed{ speed }, isShooter{ isShooter }, isBurst{ isBurst } {};

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;
private:
	float speed{ 200 };
	float fireTimer{ 0 };
	float fireRate{ 2 };
	size_t shotCount{ 0 };
	bool isShooter{ false };
	bool isBurst{ false };
};