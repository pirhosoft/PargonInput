#pragma once

#include "Pargon/Containers/Array.h"
#include "Pargon/Input/ControllerDevice.h"
#include "Pargon/Math/Vector.h"

namespace Pargon
{
	class ControllerInput
	{
	public:
		virtual ~ControllerInput() = default;

	protected:
		friend class ControllerDevice;

		virtual void Setup(const ControllerSettings& settings) = 0;
		virtual auto Update(Array<ControllerDeviceState, ControllerDevice::ControllerCount>& states, int eventIndex) -> int = 0;
		virtual void UpdateRumble(RumbleState& rumble, int i) = 0;

		void AdjustTrigger(float& value, float hardwareDeadzone, float customDeadzone);
		void AdjustStick(Vector2& value, float hardwareDeadzone, float customDeadzone);
		void UpdateButton(ControllerButton button, ControllerDeviceState& state, bool isDown);
	};
}
