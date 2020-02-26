#include "Pargon/Input/MouseDevice.h"
#include "Pargon/Input/MouseInput.h"
#include "Pargon/Serialization/StringReader.h"
#include "Pargon/Serialization/StringWriter.h"

using namespace Pargon;

void MouseDevice::Setup(Application& application)
{
	_input = CreateInput();
	_input->Setup(application);
}

void MouseDevice::Update(MouseState& state)
{
	state._state.PreviousX = state._state.X;
	state._state.PreviousY = state._state.Y;
	state._state.PreviousScrollX = state._state.ScrollX;
	state._state.PreviousScrollY = state._state.ScrollY;
	state._lastEventIndex = _input->Update(state._state, state._lastEventIndex);

	for (auto& consumedButton : state._consumedButtons)
		consumedButton = false;

	state._consumedMove = false;
	state._consumedScroll = false;
}

void MouseDevice::SetCursor(MouseCursor cursor)
{
	_cursor = cursor;
	_input->SetCursor(cursor);
}

auto MouseDevice::DoubleClickDelay() const -> Time
{
	return _input->GetDoubleClickDelay();
}
