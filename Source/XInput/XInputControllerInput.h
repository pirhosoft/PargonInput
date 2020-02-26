#pragma once

#include "Pargon/Application/EventStream.h"
#include "Pargon/Input/ControllerInput.h"

#pragma comment(lib, "xinput.lib")

namespace Pargon
{
	class XInputControllerInput : public ControllerInput
	{
	protected:
		void Setup(const ControllerSettings& settings) override;
		auto Update(Array<ControllerDeviceState, ControllerDevice::ControllerCount>& states, int eventIndex) -> int;
		void UpdateRumble(RumbleState& rumble, int i) override;

	private:
		struct XInputControllerEvent
		{
			int ControllerIndex;
			bool Pressed;
			ControllerButton Button;
		};

		ControllerSettings _settings;

		EventStream<XInputControllerEvent, 40> _controllerEvents;
	};
}
