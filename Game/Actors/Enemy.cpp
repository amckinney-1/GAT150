#include "Enemy.h"
#include "Projectile.h"
#include "Player.h"
#include "Engine.h"

void Enemy::Update(float dt)
{
	Actor::Update(dt);

	if (scene->GetActor<Player>())
	{
		Engine::Vector2 direction = scene->GetActor<Player>()->transform.position - transform.position;
		Engine::Vector2 forward = Engine::Vector2::Rotate(Engine::Vector2::right, transform.rotation);

		float turnAngle = Engine::Vector2::SignedAngle(forward, direction.Normalized());
		transform.rotation = transform.rotation + turnAngle * (4 * dt);

		float angle = Engine::Vector2::Angle(forward, direction.Normalized());

		if (isShooter)
		{
			fireTimer -= dt;

			if (isBurst && fireTimer <= 0 && angle <= Engine::DegToRad(10))
			{
				if (shotCount < 3)
				{
					shotCount++;
					fireTimer += 0.2;

					Engine::Transform t = transform;
					t.scale = 0.5f;
					scene->AddActor(std::make_unique<Projectile>(t, scene->engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("spaceMissiles_012.png", scene->engine->Get<Engine::Renderer>()), 600));

					scene->engine->Get<Engine::AudioSystem>()->PlayAudio("EnemyShoot");

				}
				else
				{
					fireTimer = fireRate;
					shotCount = 0;
				}
			}

			if (fireTimer <= 0 && angle <= Engine::DegToRad(10))
			{
				fireTimer = fireRate;

				Engine::Transform t = transform;
				t.scale = 0.5f;
				scene->AddActor(std::make_unique<Projectile>(t, scene->engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("spaceMissiles_012.png", scene->engine->Get<Engine::Renderer>()), 600));

				scene->engine->Get<Engine::AudioSystem>()->PlayAudio("EnemyShoot");
			}

		}
	}

	transform.position += Engine::Vector2::Rotate(Engine::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = Engine::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = Engine::Wrap(transform.position.y, 0.0f, 600.0f);
}

void Enemy::OnCollision(Actor* actor)
{
	//if (/*dynamic_cast<Player*>(actor) || */(dynamic_cast<Projectile*>(actor) && actor->tag == "Player"))
	//{
	//	actor->destroy = true;

	//	destroy = true;
	//	scene->engine->Get<Engine::ParticleSystem>()->Create(transform.position, 200, 1, Engine::Color::white, 100);
	//	scene->engine->Get<Engine::AudioSystem>()->PlayAudio("EnemyExplosion");

	//	Engine::Event event;
	//	event.name = "AddPoints";
	//	event.data = 300;
	//	scene->engine->Get<Engine::EventSystem>()->Notify(event);
	//}
}