#include "Pargon/Application/Application.h"
#include "Win32/Win32MouseInput.h"
#include "Pargon/Input.Win32.h"

using namespace Pargon;

auto Pargon::CreateWin32MouseInput() -> std::unique_ptr<MouseInput>
{
	return std::make_unique<Win32MouseInput>();
}

namespace
{
	auto GetMouseButton(WPARAM button) -> MouseButton
	{
		switch (button)
		{
			case MK_LBUTTON: return MouseButton::Left;
			case MK_MBUTTON: return MouseButton::Middle;
			case MK_RBUTTON: return MouseButton::Right;
			case MK_XBUTTON1: return MouseButton::XOne;
			case MK_XBUTTON2: return MouseButton::XTwo;
		}

		return MouseButton::Unknown;
	}
}

void Win32MouseInput::Setup(Application& application)
{
	_cursors[static_cast<int>(MouseCursor::Arrow)] = LoadCursor(NULL, IDC_ARROW);
	_cursors[static_cast<int>(MouseCursor::Pointer)] = LoadCursor(NULL, IDC_HAND);
	_cursors[static_cast<int>(MouseCursor::Loading)] = LoadCursor(NULL, IDC_APPSTARTING);
	_cursors[static_cast<int>(MouseCursor::Crosshair)] = LoadCursor(NULL, IDC_CROSS);
	_cursors[static_cast<int>(MouseCursor::Help)] = LoadCursor(NULL, IDC_HELP);
	_cursors[static_cast<int>(MouseCursor::Text)] = LoadCursor(NULL, IDC_IBEAM);
	_cursors[static_cast<int>(MouseCursor::Move)] = LoadCursor(NULL, IDC_SIZEALL);
	_cursors[static_cast<int>(MouseCursor::ResizeWE)] = LoadCursor(NULL, IDC_SIZEWE);
	_cursors[static_cast<int>(MouseCursor::ResizeNS)] = LoadCursor(NULL, IDC_SIZENS);
	_cursors[static_cast<int>(MouseCursor::ResizeNWSE)] = LoadCursor(NULL, IDC_SIZENWSE);
	_cursors[static_cast<int>(MouseCursor::ResizeNESW)] = LoadCursor(NULL, IDC_SIZENESW);
	_cursors[static_cast<int>(MouseCursor::Wait)] = LoadCursor(NULL, IDC_WAIT);
	_cursors[static_cast<int>(MouseCursor::Disabled)] = LoadCursor(NULL, IDC_NO);
	_cursors[static_cast<int>(MouseCursor::None)] = NULL;

	_application = application.Data();
	_application->AddHandler(WM_SETCURSOR, [=](WPARAM w, LPARAM l){ return SetCursor(w, l); });
	_application->AddHandler(WM_LBUTTONDOWN, [=](WPARAM w, LPARAM l){ return LeftMousePressed(w, l); });
	_application->AddHandler(WM_LBUTTONUP, [=](WPARAM w, LPARAM l) { return LeftMouseReleased(w, l); });
	_application->AddHandler(WM_MBUTTONDOWN, [=](WPARAM w, LPARAM l) { return MiddleMousePressed(w, l); });
	_application->AddHandler(WM_MBUTTONUP, [=](WPARAM w, LPARAM l) { return MiddleMouseReleased(w, l); });
	_application->AddHandler(WM_RBUTTONDOWN, [=](WPARAM w, LPARAM l) { return RightMousePressed(w, l); });
	_application->AddHandler(WM_RBUTTONUP, [=](WPARAM w, LPARAM l) { return RightMouseReleased(w, l); });
	_application->AddHandler(WM_XBUTTONDOWN, [=](WPARAM w, LPARAM l) { return ExtraMousePressed(w, l); });
	_application->AddHandler(WM_XBUTTONUP, [=](WPARAM w, LPARAM l) { return ExtraMouseReleased(w, l); });
	_application->AddHandler(WM_MOUSEWHEEL, [=](WPARAM w, LPARAM l) { return MouseScrolledVertically(w, l); });
	_application->AddHandler(WM_MOUSEHWHEEL, [=](WPARAM w, LPARAM l) { return MouseScrolledHorizontally(w, l); });
}

auto Win32MouseInput::Update(MouseDeviceState& state, int eventIndex) -> int
{
	POINT mousePosition;
	RECT windowBounds;

	GetCursorPos(&mousePosition);
	ScreenToClient(_application->Window(), &mousePosition);
	GetClientRect(_application->Window(), &windowBounds);

	if (mousePosition.x >= 0 && mousePosition.x < (windowBounds.right - windowBounds.left) && mousePosition.y >= 0 && mousePosition.y < (windowBounds.bottom - windowBounds.top))
	{
		state.X = static_cast<float>(mousePosition.x);
		state.Y = static_cast<float>(windowBounds.bottom - mousePosition.y);
	}

	state.ScrollX = _scrollX;
	state.ScrollY = _scrollY;

	auto currentIndex = _events.CurrentIndex();

	for (auto i = 0u; i < static_cast<int>(MouseButton::Count); i++)
	{
		state.Pressed.SetItem(i, false);
		state.Released.SetItem(i, false);
	}

	while (eventIndex != currentIndex)
	{
		auto& event = _events.Event(eventIndex);

		auto button = GetMouseButton(event.Button);

		state.Down.SetItem(static_cast<int>(button), event.Pressed);

		if (event.Pressed)
			state.Pressed.SetItem(static_cast<int>(button), true);
		else
			state.Released.SetItem(static_cast<int>(button), true);

		eventIndex = _events.NextIndex(eventIndex);
	}

	return eventIndex;
}

void Win32MouseInput::SetCursor(MouseCursor cursor)
{
	_cursor = cursor;
}

auto Win32MouseInput::SetCursor(WPARAM w, LPARAM l) -> bool
{
	POINT mousePosition;
	RECT clientArea;

	GetCursorPos(&mousePosition);
	GetClientRect(_application->Window(), &clientArea);
	ScreenToClient(_application->Window(), &mousePosition);

	if (mousePosition.x < 0 || mousePosition.x > clientArea.right || mousePosition.y < 0 || mousePosition.y > clientArea.bottom)
		return false;

	::SetCursor(_cursors[static_cast<int>(_cursor)]);
	return true;
}

auto Win32MouseInput::GetDoubleClickDelay() -> Time
{
	auto ms = GetDoubleClickTime();
	return Time::FromMilliseconds(ms);
}

auto Win32MouseInput::LeftMousePressed(WPARAM w, LPARAM l) -> bool
{
	_events.AddEvent({ true, MK_LBUTTON });
	return true;
}

auto Win32MouseInput::LeftMouseReleased(WPARAM w, LPARAM l) -> bool
{
	_events.AddEvent({ false, MK_LBUTTON });
	return true;
}

auto Win32MouseInput::MiddleMousePressed(WPARAM w, LPARAM l) -> bool
{
	_events.AddEvent({ true, MK_MBUTTON });
	return true;
}

auto Win32MouseInput::MiddleMouseReleased(WPARAM w, LPARAM l) -> bool
{
	_events.AddEvent({ false, MK_MBUTTON });
	return true;
}

auto Win32MouseInput::RightMousePressed(WPARAM w, LPARAM l) -> bool
{
	_events.AddEvent({ true, MK_RBUTTON });
	return true;
}

auto Win32MouseInput::RightMouseReleased(WPARAM w, LPARAM l) -> bool
{
	_events.AddEvent({ false, MK_RBUTTON });
	return true;
}

auto Win32MouseInput::ExtraMousePressed(WPARAM w, LPARAM l) -> bool
{
	if ((w & MK_XBUTTON1) != 0)
		_events.AddEvent({ true, MK_XBUTTON1 });
	else if ((w & MK_XBUTTON2) != 0)
		_events.AddEvent({ true, MK_XBUTTON2 });

	return true;
}

auto Win32MouseInput::ExtraMouseReleased(WPARAM w, LPARAM l) -> bool
{
	if ((w & MK_XBUTTON1) != 0)
		_events.AddEvent({ false, MK_XBUTTON1 });
	else if ((w & MK_XBUTTON2) != 0)
		_events.AddEvent({ false, MK_XBUTTON2 });

	return true;
}

auto Win32MouseInput::MouseScrolledVertically(WPARAM w, LPARAM l) -> bool
{
	auto amount = static_cast<float>(GET_WHEEL_DELTA_WPARAM(w));
	_scrollY += amount / 120.0f;

	return true;
}

auto Win32MouseInput::MouseScrolledHorizontally(WPARAM w, LPARAM l) -> bool
{
	auto amount = static_cast<float>(GET_WHEEL_DELTA_WPARAM(w));
	_scrollX += amount / 120.0f;

	return true;
}
