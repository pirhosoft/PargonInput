#pragma once

#include "Pargon/Input/KeyboardDevice.h"

namespace Pargon
{
	class KeyboardInput
	{
	public:
		virtual ~KeyboardInput() = default;

	protected:
		friend class KeyboardDevice;

		virtual void Setup(Application& application) = 0;
		virtual auto Update(KeyboardDeviceState& state, int eventIndex) -> int = 0;
		virtual void ShowKeyboard() = 0;
		virtual void HideKeyboard() = 0;
	};
}
