#include "ProjectileComponent.h"
#include "Engine.h"

using namespace nEngine;

void ProjectileComponent::Update()
{
	/*lifetime -= owner->scene->engine->time.deltaTime;
	destroy = (lifetime <= 0);

	transform.position += Engine::Vector2::Rotate(Engine::Vector2::right, transform.rotation) * speed * dt;*/
}

bool ProjectileComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool ProjectileComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	JSON_READ(value, lifetime);

	return true;
}
