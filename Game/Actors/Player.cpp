#include "Player.h"
#include "Math/MathUtils.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Asteroid.h"
#include "Engine.h"
#include <SDL.h>

void Player::Initialize()
{
	/*
	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localPosition = Engine::Vector2{ 2, 3 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = Engine::Vector2{ 2, -3 };
	AddChild(std::move(locator));

	std::unique_ptr engine = std::make_unique<Actor>(Engine::Transform{}, scene->engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("spaceShips_003.png", scene->engine->Get<Engine::Renderer>()));
	engine->transform.localPosition = Engine::Vector2{ -2, 0 };
	//engine->transform.localRotation = Engine::DegToRad(180);
	AddChild(std::move(engine));
	*/
}

void Player::Update(float dt)
{
	Actor::Update(dt);

	//children[2]->transform.localRotation += 5 * dt;

	// movement
	float thrust = 0;
	if (scene->engine->Get<Engine::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == Engine::InputSystem::eKeyState::Held) transform.rotation -= 5 * dt;
	if (scene->engine->Get<Engine::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == Engine::InputSystem::eKeyState::Held) transform.rotation += 5 * dt;
	if (scene->engine->Get<Engine::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == Engine::InputSystem::eKeyState::Held) thrust = speed;

	// acceleration
	Engine::Vector2 acceleration;
	acceleration += Engine::Vector2::Rotate(Engine::Vector2::right, transform.rotation) * thrust;
	//Engine::Vector2 gravity = (Engine::Vector2{ 400, 300 } - transform.position).Normalized() * 200; // Swap direction and normalized for repulsion 
	//Engine::Vector2 gravity = Engine::Vector2::down * 200;
	//acceleration += gravity;
	
	velocity += acceleration * dt;
	transform.position += velocity * dt;

	velocity *= 0.985f; // Inertial Dampening

	transform.position.x = Engine::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = Engine::Wrap(transform.position.y, 0.0f, 600.0f);

	// fire
	fireTimer -= dt;
	if (fireTimer <= 0 && scene->engine->Get<Engine::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == Engine::InputSystem::eKeyState::Pressed)
	{
		fireTimer = fireRate;
		std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(transform, scene->engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("spaceMissiles_012.png", scene->engine->Get<Engine::Renderer>()), 600);
		projectile->tag = "Player";
		scene->AddActor(std::move(projectile));
		scene->engine->Get<Engine::AudioSystem>()->PlayAudio("PlayerShoot");

		/*
		{

			Engine::Transform t = children[0]->transform;
			t.scale = 0.5f;
			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("spaceMissiles_012.png", scene->engine->Get<Engine::Renderer>()), 600);
			projectile->tag = "Player";
			scene->AddActor(std::move(projectile));
			scene->engine->Get<Engine::AudioSystem>()->PlayAudio("PlayerShoot");
		}
		{
			
			Engine::Transform t = children[1]->transform;
			t.scale = 0.5f;
			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("spaceMissiles_012.png", scene->engine->Get<Engine::Renderer>()), 600);
			projectile->tag = "Player";
			scene->AddActor(std::move(projectile));
			scene->engine->Get<Engine::AudioSystem>()->PlayAudio("PlayerShoot");
		}
		*/
	}

	//std::vector<Engine::Color> colors = { Engine::Color::purple, Engine::Color::cyan, Engine::Color::blue };
	//scene->engine->Get<Engine::ParticleSystem>()->Create(children[2]->transform.position, 3, 2, colors, 75, children[2]->transform.rotation + Engine::DegToRad(180), Engine::DegToRad(30));
}

void Player::OnCollision(Actor* actor)
{
	return;

	if (dynamic_cast<Enemy*>(actor) || dynamic_cast<Asteroid*>(actor) || (dynamic_cast<Projectile*>(actor) && actor->tag != "Player"))
	{
		destroy = true;
		scene->engine->Get<Engine::ParticleSystem>()->Create(transform.position, 200, 1, scene->engine->Get<Engine::ResourceSystem>()->Get<Engine::Texture>("particle01.png", scene->engine->Get<Engine::Renderer>()), 100);
		scene->engine->Get<Engine::AudioSystem>()->PlayAudio("PlayerExplosion");

		Engine::Event event;
		event.name = "PlayerDead";
		event.data = std::string("yes i'm dead!");
		scene->engine->Get<Engine::EventSystem>()->Notify(event);
	}
}
