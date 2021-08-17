#pragma once
#include "Component.h"

namespace Engine
{
	class Renderer;

	class GraphicsComponent : public Component
	{
	public:
		virtual void Draw(Renderer* renderer) = 0;

	};
}