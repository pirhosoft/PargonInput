#include "Pargon/Containers/List.h"
#include "Pargon/Containers/String.h"
#include "Pargon/Application/Log.h"
#include "Pargon/Input/ControllerDevice.h"
#include "Pargon/Input/ControllerInput.h"
#include "Pargon/Math/Arithmetic.h"
#include "Pargon/Serialization/StringReader.h"
#include "Pargon/Serialization/StringWriter.h"

using namespace Pargon;

void ControllerDevice::Setup(ControllerSettings&& settings)
{
	_input = CreateInput();
	_input->Setup(settings);

	_settings = std::move(settings);
}

void ControllerDevice::Update(ControllerState& state)
{
	state._lastEventIndex = _input->Update(state._states, state._lastEventIndex);

	for (auto i = 0; i < ControllerDevice::ControllerCount; ++i)
	{
		state._consumedLeftStick.SetItem(i, false);
		state._consumedRightStick.SetItem(i, false);
		state._consumedLeftTrigger.SetItem(i, false);
		state._consumedRightTrigger.SetItem(i, false);

		for (auto& consumedButton : state._consumedButtons.Item(i))
			consumedButton = false;
	}
}

void ControllerDevice::SetRumble(int index, float highFrequency, float lowFrequency)
{
	if (index < _rumble.Count())
	{
		auto& state = _rumble.Item(index);

		state.HighFrequencyRumble = Minimum(highFrequency, 65.0f);
		state.LowFrequencyRumble = Minimum(lowFrequency, 65.0f);

		_input->UpdateRumble(state, index);
	}
}

void ControllerDevice::UpdateRumble(float elapsed)
{
	for (auto i = 0; i < _rumble.Count(); i++)
	{
		auto& state = _rumble.Item(i);
		auto step = 50.0f * elapsed;

		if (state.HighFrequencyRumble > 0.0f || state.LowFrequencyRumble > 0.0f)
		{
			state.HighFrequencyRumble -= step;
			state.LowFrequencyRumble -= step;

			if (state.HighFrequencyRumble < 0.0f && state.LowFrequencyRumble < 0.0f)
			{
				state.HighFrequencyRumble = 0.0f;
				state.LowFrequencyRumble = 0.0f;
				_input->UpdateRumble(state, i);
			}
		}
	}
}
