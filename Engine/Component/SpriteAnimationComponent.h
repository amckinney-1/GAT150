#pragma once
#include "Component/SpriteComponent.h"
#include <SDL.h>

namespace Engine
{
	class SpriteAnimationComponent : public SpriteComponent
	{
	public:
		void Update() override;
		void Draw(Renderer* renderer) override;

		// Inherited via SpriteComponent
		virtual bool Write(const rapidjson::Value& value) const override;

		virtual bool Read(const rapidjson::Value& value) override;

	public:
		int frame{ 0 };
		int fps{ 0 };

		int numFramesX{ 0 };
		int numFramesY{ 0 };
		int startFrame{ 0 };
		int endFrame{ 0 };

		float frameTimer{ 0 };
		float frameTime{ 0 };

		SDL_Rect rect;
	};
}