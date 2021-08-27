#include "EnemyComponent.h"
#include "Engine.h"

using namespace Engine;

void EnemyComponent::Update()
{
	Actor* player = owner->scene->FindActor("Player");
	if (player)
	{
		Vector2 direction = player->transform.position - owner->transform.position;
		Vector2 force = direction.Normalized() * speed;

		PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
		assert(physicsComponent);

		physicsComponent->ApplyForce(force);
	}
}

bool EnemyComponent::Write(const rapidjson::Value& value) const
{
	JSON_READ(value, speed);

	return false;
}

bool EnemyComponent::Read(const rapidjson::Value& value)
{
	return false;
}
