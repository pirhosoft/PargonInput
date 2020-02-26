#include "Pargon/Application/Application.h"
#include "Pargon/Input/MouseToTouchEmulator.h"

using namespace Pargon;

void MouseToTouchEmulator::Setup(Application& application)
{
	_mouse.Setup(application);
}

auto MouseToTouchEmulator::Update(TouchDeviceState& state, int eventIndex) -> int
{
	_mouse.Update(_state);

	auto& first = state.Touches.Item(0);
	auto& second = state.Touches.Item(1);

	second.Down = _state.IsButtonDown(MouseButton::Right);
	second.Pressed = _state.WasButtonJustPressed(MouseButton::Right);
	second.Released = _state.WasButtonJustReleased(MouseButton::Right);

	first.Down = second.Down || _state.IsButtonDown(MouseButton::Left);
	first.Pressed = second.Pressed || _state.WasButtonJustPressed(MouseButton::Left);
	first.Released = second.Released || _state.WasButtonJustReleased(MouseButton::Left);

	if (second.Down)
	{
		first.Position = { _state.MouseX() - 25, _state.MouseY() };
		second.Position = { _state.MouseX() + 25, _state.MouseY() };

		first.Position.X -= _state.ScrollY();
		second.Position.X += _state.ScrollY();;
	}
	else if (first.Down)
	{
		first.Position.X = _state.MouseX();
		first.Position.Y = _state.MouseY();
	}

	return eventIndex;
}
