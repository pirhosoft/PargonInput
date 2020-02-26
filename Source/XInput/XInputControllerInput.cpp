#include "Pargon/Math/Arithmetic.h"
#include "Pargon/Application.Win32.h"
#include "XInput/XInputControllerInput.h"
#include "Pargon/Input.XInput.h"

#include <Xinput.h>

using namespace Pargon;

auto Pargon::CreateXInputControllerInput() -> std::unique_ptr<ControllerInput>
{
	return std::make_unique<XInputControllerInput>();
}

namespace
{
	auto GetControllerButton(WORD button) -> ControllerButton
	{
		switch (button)
		{
			case VK_PAD_A: return ControllerButton::A;
			case VK_PAD_B: return ControllerButton::B;
			case VK_PAD_X: return ControllerButton::X;
			case VK_PAD_Y: return ControllerButton::Y;
			case VK_PAD_RSHOULDER: return ControllerButton::RightBumper;
			case VK_PAD_LSHOULDER: return ControllerButton::LeftBumper;
			case VK_PAD_DPAD_UP: return ControllerButton::Up;
			case VK_PAD_DPAD_DOWN: return ControllerButton::Down;
			case VK_PAD_DPAD_LEFT: return ControllerButton::Left;
			case VK_PAD_DPAD_RIGHT: return ControllerButton::Right;
			case VK_PAD_START: return ControllerButton::Start;
			case VK_PAD_BACK: return ControllerButton::Back;
			case VK_PAD_LTHUMB_PRESS: return ControllerButton::LeftStick;
			case VK_PAD_RTHUMB_PRESS: return ControllerButton::RightStick;
		}

		return ControllerButton::Unknown;
	}
}

void XInputControllerInput::Setup(const ControllerSettings& settings)
{
	_settings = settings;
}

auto XInputControllerInput::Update(Array<ControllerDeviceState, ControllerDevice::ControllerCount>& states, int eventIndex) -> int
{
	long status;
	XINPUT_KEYSTROKE keystroke;

	status = XInputGetKeystroke(XUSER_INDEX_ANY, 0, &keystroke);
	while (status == ERROR_SUCCESS)
	{
		if ((keystroke.Flags & XINPUT_KEYSTROKE_REPEAT) == 0)
		{
			auto index = static_cast<int>(keystroke.UserIndex);
			auto pressed = (keystroke.Flags & XINPUT_KEYSTROKE_KEYDOWN) != 0;
			auto button = GetControllerButton(keystroke.VirtualKey);

			if (button != ControllerButton::Unknown)
				_controllerEvents.AddEvent({ index, pressed, button });
		}

		status = XInputGetKeystroke(XUSER_INDEX_ANY, {}, &keystroke);
	}

	for (auto i = 0; i < Minimum(ControllerDevice::ControllerCount, 4); i++)
	{
		XINPUT_STATE xinput = { 0 };
		auto result = XInputGetState(i, &xinput);

		auto& state = states.Item(i);
		auto connected = result == ERROR_SUCCESS;

		state.Connected = connected;

		if (state.Connected)
		{
			state.LeftTrigger = xinput.Gamepad.bLeftTrigger / 255.0f;
			state.RightTrigger = xinput.Gamepad.bRightTrigger / 255.0f;
			state.LeftStick = { xinput.Gamepad.sThumbLX / 32768.0f, xinput.Gamepad.sThumbLY / 32768.0f };
			state.RightStick = { xinput.Gamepad.sThumbRX / 32768.0f, xinput.Gamepad.sThumbRY / 32768.0f };

			AdjustTrigger(state.LeftTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD / 255.0f, _settings.TriggerDeadzone);
			AdjustTrigger(state.RightTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD / 255.0f, _settings.TriggerDeadzone);
			AdjustStick(state.LeftStick, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / 32767.0f, _settings.StickDeadzone);
			AdjustStick(state.RightStick, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / 32767.0f, _settings.StickDeadzone);
		}
		else
		{
			state.LeftTrigger = 0.0f;
			state.RightTrigger = 0.0f;
			state.LeftStick = { 0.0f, 0.0f };
			state.RightStick = { 0.0f, 0.0f };

			for (auto b = 0; b < static_cast<int>(ControllerButton::Count); b++)
				state.Down.SetItem(b, false);
		}

		for (auto b = 0; b < static_cast<int>(ControllerButton::LeftTrigger); b++)
		{
			state.Pressed.SetItem(b, false);
			state.Released.SetItem(b, false);
		}

		UpdateButton(ControllerButton::LeftTrigger, state, state.LeftTrigger > _settings.TriggerPressThreshold);
		UpdateButton(ControllerButton::RightTrigger, state, state.RightTrigger > _settings.TriggerPressThreshold);
		UpdateButton(ControllerButton::LeftStickLeft, state, state.LeftStick.X < -_settings.StickPressThreshold);
		UpdateButton(ControllerButton::LeftStickRight, state, state.LeftStick.X > _settings.StickPressThreshold);
		UpdateButton(ControllerButton::LeftStickDown, state, state.LeftStick.Y < -_settings.StickPressThreshold);
		UpdateButton(ControllerButton::LeftStickUp, state, state.LeftStick.Y > _settings.StickPressThreshold);
		UpdateButton(ControllerButton::RightStickLeft, state, state.RightStick.X < -_settings.StickPressThreshold);
		UpdateButton(ControllerButton::RightStickRight, state, state.RightStick.X > _settings.StickPressThreshold);
		UpdateButton(ControllerButton::RightStickDown, state, state.RightStick.Y < -_settings.StickPressThreshold);
		UpdateButton(ControllerButton::RightStickUp, state, state.RightStick.Y > _settings.StickPressThreshold);
	}

	while (eventIndex != _controllerEvents.CurrentIndex())
	{
		auto& event = _controllerEvents.Event(eventIndex);
		auto& state = states.Item(event.ControllerIndex);

		state.Down.SetItem(static_cast<int>(event.Button), event.Pressed);

		if (event.Pressed)
			state.Pressed.SetItem(static_cast<int>(event.Button), true);
		else
			state.Released.SetItem(static_cast<int>(event.Button), true);

		eventIndex = _controllerEvents.NextIndex(eventIndex);
	}

	return eventIndex;
}

void XInputControllerInput::UpdateRumble(RumbleState& rumble, int i)
{
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = static_cast<WORD>(rumble.LowFrequencyRumble * 1000.0f);
	vibration.wRightMotorSpeed = static_cast<WORD>(rumble.HighFrequencyRumble * 1000.0f);
	XInputSetState(i, &vibration);
}
