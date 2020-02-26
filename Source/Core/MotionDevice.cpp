#include "Pargon/Input/MotionDevice.h"
#include "Pargon/Input/MotionInput.h"

using namespace Pargon;

void MotionDevice::Setup()
{
	_input = CreateInput();
}

void MotionDevice::Update(MotionState& state)
{
	_input->Update(state._state);
}
