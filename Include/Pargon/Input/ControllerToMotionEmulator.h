#pragma once

#include "Pargon/Input/ControllerDevice.h"
#include "Pargon/Input/MotionInput.h"

namespace Pargon
{
	class ControllerToMotionEmulator : public MotionInput
	{
	protected:
		void Setup();
		void Update(MotionDeviceState& state) override;

	private:
		ControllerDevice _controller;
		ControllerState _state;
	};
}
