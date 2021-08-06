#pragma once
#include "Framework/System.h"
#include <SDL.h>
#include <vector>

namespace Engine
{
	class InputSystem : public System
	{
	public:
		enum class eKeyState
		{
			Idle,
			Pressed,
			Held,
			Released
		};

	public:
		virtual void Startup();
		virtual void Shutdown();
		virtual void Update(float dt);

		eKeyState GetKeyState(int id);
		bool IsKeyDown(int id);
		bool IsPreviousKeyDown(int id);

	private:
		std::vector<Uint8> keyboardState;
		std::vector<Uint8> prevKeyboardState;
		int numKeys;

	};
}