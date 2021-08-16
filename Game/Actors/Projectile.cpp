#include "Projectile.h"
#include "Math/MathUtils.h"
#include "Engine.h"

void Projectile::Update(float dt)
{
	Actor::Update(dt);

	lifetime -= dt;
	destroy = (lifetime <= 0);

	transform.position += Engine::Vector2::Rotate(Engine::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = Engine::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = Engine::Wrap(transform.position.y, 0.0f, 600.0f);

	//std::vector<Engine::Color> colors{ Engine::Color::white, Engine::Color::orange };
	//scene->engine->Get<Engine::ParticleSystem>()->Create(transform.position, 10, 0.5f, colors[Engine::RandomRangeInt(0, colors.size())], 50);
}