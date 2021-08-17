#pragma once
#include "Vector2.h"
#include "Matrix33.h"
#include "Matrix22.h"

namespace Engine
{
	struct Transform {
		Vector2 position;
		float rotation{ 0 };
		float scale{ 1 };

		Vector2 localPosition;
		float localRotation{ 0 };
		float localScale{ 1 };

		Matrix33 matrix;

		Transform() {}
		Transform(const Vector2& position, float rotation = 0, float scale = 1) : position{position}, rotation{rotation}, scale{scale} {}

		void Update();
		void Update(const Matrix33& mx);
	};
}