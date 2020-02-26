#include "Pargon/Math/Rotation.h"
#include "Pargon/Math/Trigonometry.h"
#include "Pargon/Input/ControllerToMotionEmulator.h"
#include "Pargon/Math/Angle.h"

using namespace Pargon;

void ControllerToMotionEmulator::Setup()
{
	_controller.Setup({});
}

void ControllerToMotionEmulator::Update(MotionDeviceState& state)
{
	_controller.Update(_state);

	if (_state.IsControllerConnected(0))
	{
		auto leftStick = _state.LeftStick(0);
		auto rightStick = _state.RightStick(0);

		state.Roll = Rotation::FromRadians(Pi.InRadians() * leftStick.X);
		state.Pitch = Rotation::FromRadians(Pi.InRadians() * leftStick.Y);
		state.Yaw = Rotation::FromRadians(Pi.InRadians() * rightStick.X);
	}
	else
	{
		state.Yaw = 0.0_radians;
		state.Pitch = 0.0_radians;
		state.Roll = 0.0_radians;
	}
}
