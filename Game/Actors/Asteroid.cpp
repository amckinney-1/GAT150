#include "Asteroid.h"
#include "Player.h"
#include "Projectile.h"
#include "Math/Random.h"
#include "Engine.h"

void Asteroid::Update(float dt)
{
	Actor::Update(dt);

	transform.rotation += (1 + Engine::RandomRange(1, 5));

	transform.position += Engine::Vector2(Engine::RandomRangeInt(-1, 1), Engine::RandomRangeInt(-1, 1)) * speed * dt;

	transform.position.x = Engine::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = Engine::Wrap(transform.position.y, 0.0f, 600.0f);
}

void Asteroid::OnCollision(Actor* actor)
{
	if (dynamic_cast<Player*>(actor) || (dynamic_cast<Projectile*>(actor) && actor->tag == "Player"))
	{
		actor->destroy = true;

		destroy = true;
		scene->engine->Get<Engine::ParticleSystem>()->Create(transform.position, 200, 1, scene->engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("particle01.png"), 100);
		scene->engine->Get<Engine::AudioSystem>()->PlayAudio("EnemyExplosion");

		Engine::Event event;
		event.name = "AddPoints";
		event.data = 100;
		scene->engine->Get<Engine::EventSystem>()->Notify(event);
	}
}