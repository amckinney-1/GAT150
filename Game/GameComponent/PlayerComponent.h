#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"

class PlayerComponent : public Engine::Component
{
public:
	void Create() override;
	virtual void Update() override;

	virtual void OnCollisionEnter(const Engine::Event& event);
	virtual void OnCollisionExit(const Engine::Event& event);

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

public:
	float speed{ 0 };
	std::list<Engine::Actor*> contacts;

};