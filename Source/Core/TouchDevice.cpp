#include "Pargon/Input/TouchDevice.h"
#include "Pargon/Input/TouchInput.h"

using namespace Pargon;

void TouchDevice::Setup(Application& application)
{
	_input = CreateInput();
	_input->Setup(application);
}

void TouchDevice::Update(TouchState& state)
{
	for (auto i = 0; i < MaxTouches; i++)
	{
		auto& touch = state._state.Touches.Item(i);
		touch.PreviousPosition = touch.Position;
	}

	state._lastEventIndex = _input->Update(state._state, state._lastEventIndex);

	for (auto& consumedTouch : state._consumedTouches)
		consumedTouch = false;
}
