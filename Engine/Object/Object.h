#pragma once

namespace Engine
{
	class Object
	{
	public:
		virtual ~Object() {}

		virtual void Create() {}
	};
}