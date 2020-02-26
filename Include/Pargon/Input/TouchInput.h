#pragma once

#include "Pargon/Input/TouchDevice.h"

namespace Pargon
{
	class TouchInput
	{
	public:
		virtual ~TouchInput() = default;

	protected:
		friend class TouchDevice;

		virtual void Setup(Application& application) = 0;
		virtual auto Update(TouchDeviceState& state, int eventIndex) -> int = 0;
	};
}
