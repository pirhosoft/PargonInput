#pragma once

#include "Pargon/Application/EventStream.h"
#include "Pargon/Input/MouseInput.h"
#include "Pargon/Application.Win32.h"

namespace Pargon
{
	class Win32MouseInput: public MouseInput
	{
	protected:
		void Setup(Application& application) override;
		auto Update(MouseDeviceState& state, int eventIndex) -> int override;
		
		void SetCursor(MouseCursor cursor) override;
		auto GetDoubleClickDelay() -> Time override;

	private:
		struct MouseEvent
		{
			bool Pressed;
			WPARAM Button;
		};

		ApplicationData* _application;
		HCURSOR _cursors[14];
		MouseCursor _cursor = MouseCursor::Arrow;
		EventStream<MouseEvent, 10> _events;

		float _scrollX = 0.0f;
		float _scrollY = 0.0f;

		auto SetCursor(WPARAM w, LPARAM l) -> bool;
		auto LeftMousePressed(WPARAM w, LPARAM l) -> bool;
		auto LeftMouseReleased(WPARAM w, LPARAM l) -> bool;
		auto MiddleMousePressed(WPARAM w, LPARAM l) -> bool;
		auto MiddleMouseReleased(WPARAM w, LPARAM l) -> bool;
		auto RightMousePressed(WPARAM w, LPARAM l) -> bool;
		auto RightMouseReleased(WPARAM w, LPARAM l) -> bool;
		auto ExtraMousePressed(WPARAM w, LPARAM l) -> bool;
		auto ExtraMouseReleased(WPARAM w, LPARAM l) -> bool;
		auto MouseScrolledVertically(WPARAM w, LPARAM l) -> bool;
		auto MouseScrolledHorizontally(WPARAM w, LPARAM l) -> bool;
	};
}
