#pragma once

#include "Pargon/Input/MouseDevice.h"
#include "Pargon/Input/TouchInput.h"

namespace Pargon
{
	class MouseToTouchEmulator : public TouchInput
	{
	protected:
		void Setup(Application& application) override;
		auto Update(TouchDeviceState& state, int eventIndex) -> int override;

	private:
		MouseDevice _mouse;
		MouseState _state;
	};
}
