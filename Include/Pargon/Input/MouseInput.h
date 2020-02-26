#pragma once

#include "Pargon/Input/MouseDevice.h"
#include "Pargon/Types/Time.h"

namespace Pargon
{
	class MouseInput
	{
	public:
		virtual ~MouseInput() = default;
		
	protected:
		friend class MouseDevice;
		friend class MouseToTouchEmulator;

		virtual void Setup(Application& application) = 0;
		virtual auto Update(MouseDeviceState& state, int eventIndex) -> int = 0;

		virtual void SetCursor(MouseCursor cursor) = 0;
		virtual auto GetDoubleClickDelay() -> Time = 0;
	};
}
