#pragma once
#include "Component/Component.h"

class ProjectileComponent : public nEngine::Component
{
public:
	std::unique_ptr<Object> Clone() const { return std::make_unique<ProjectileComponent>(*this); }

	virtual void Update() override;

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

public:
	float speed{ 0 };
	float lifetime{ 0 };
};