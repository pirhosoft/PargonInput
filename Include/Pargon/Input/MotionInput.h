#pragma once

#include "Pargon/Input/MotionDevice.h"

namespace Pargon
{
	class MotionInput
	{
	public:
		virtual ~MotionInput() = default;

	protected:
		friend class MotionDevice;

		virtual void Update(MotionDeviceState& state) = 0;
	};
}
