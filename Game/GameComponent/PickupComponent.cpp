#include "PickupComponent.h"
#include "Engine.h"

using namespace nEngine;

PickupComponent::~PickupComponent()
{
	//
}

void PickupComponent::Create()
{
	//
}

void PickupComponent::Update()
{
	//
}

bool PickupComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool PickupComponent::Read(const rapidjson::Value& value)
{
	return true;
}
