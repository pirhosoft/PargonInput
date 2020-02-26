#include "Pargon/Input/ControllerInput.h"
#include "Pargon/Math/Arithmetic.h"

using namespace Pargon;

void ControllerInput::AdjustTrigger(float& value, float hardwareDeadzone, float customDeadzone)
{
	auto deadzone = Maximum(hardwareDeadzone, customDeadzone);
	
	if (value > 1.0f)
		value = 1.0f;
	else if (value < deadzone)
		value = 0.0f;
	else
		value = (value - deadzone) / (1.0f - deadzone);
}

void ControllerInput::AdjustStick(Vector2& value, float hardwareDeadzone, float customDeadzone)
{
	auto deadzone = Maximum(hardwareDeadzone, customDeadzone);
	
	auto magnitude = value.GetLength();

	if (magnitude > 1.0f)
		magnitude = 1.0f;
	else if (magnitude < deadzone)
		magnitude = 0.0f;
	else
		magnitude = (magnitude - deadzone) / (1.0f - deadzone);

	value.Normalize();
	value *= magnitude;
}

void ControllerInput::UpdateButton(ControllerButton button, ControllerDeviceState& state, bool isDown)
{
	auto index = static_cast<unsigned int>(button);
	auto wasDown = state.Down.Item(index);

	state.Pressed.SetItem(index, !wasDown && isDown);
	state.Down.SetItem(index, isDown);
	state.Released.SetItem(index, wasDown && !isDown);
}
