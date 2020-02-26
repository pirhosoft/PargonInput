#pragma once

#include "Pargon/Application/EventStream.h"
#include "Pargon/Input/KeyboardInput.h"
#include "Pargon/Application.Win32.h"

namespace Pargon
{
	class Win32KeyboardInput : public KeyboardInput
	{
	protected:
		void Setup(Application& application) override;
		auto Update(KeyboardDeviceState& state, int eventIndex) -> int override;
		void ShowKeyboard() override;
		void HideKeyboard() override;

	private:
		struct KeyboardEvent
		{
			bool Pressed;
			WPARAM Key;
		};

		ApplicationData* _application;
		EventStream<KeyboardEvent, 100> _events;

		auto KeyPressed(WPARAM w, LPARAM l) -> bool;
		auto KeyReleased(WPARAM w, LPARAM l) -> bool;
	};
}
