#pragma once

#include "Pargon/Containers/Array.h"
#include "Pargon/Types/Time.h"

namespace Pargon
{
	class Application;
	class StringWriter;
	class StringReader;
	class MouseInput;

	enum class MouseCursor
	{
		Arrow,
		Pointer,
		Loading,
		Crosshair,
		Help,
		Text,
		Move,
		ResizeWE,
		ResizeNS,
		ResizeNWSE,
		ResizeNESW,
		Wait,
		Disabled,
		None
	};

	enum class MouseButton
	{
		Left, Right, Middle,
		XOne, XTwo,
		Unknown, Count
	};

	struct MouseDeviceState
	{
		float PreviousX = 0;
		float PreviousY = 0;
		float X = 0;
		float Y = 0;
		float PreviousScrollX = 0.0f;
		float PreviousScrollY = 0.0f;
		float ScrollX = 0.0f;
		float ScrollY = 0.0f;
		Array<bool, static_cast<int>(MouseButton::Count)> Pressed;
		Array<bool, static_cast<int>(MouseButton::Count)> Released;
		Array<bool, static_cast<int>(MouseButton::Count)> Down;
	};

	class MouseState
	{
	public:
		auto PreviousMouseX() const -> float;
		auto PreviousMouseY() const -> float;
		auto MouseX() const -> float;
		auto MouseY() const -> float;
		auto PreviousScrollX() const -> float;
		auto PreviousScrollY() const -> float;
		auto ScrollX() const -> float;
		auto ScrollY() const -> float;
		auto IsButtonDown(MouseButton button) const -> bool;
		auto WasButtonJustPressed(MouseButton button) const -> bool;
		auto WasButtonJustReleased(MouseButton button) const -> bool;

		void ConsumeAll();
		void ConsumeButton(MouseButton button);
		void ConsumeMove();
		void ConsumeScroll();

		auto WasButtonConsumed(MouseButton button) const -> bool;
		auto WasMoveConsumed() const -> bool;
		auto WasScrollConsumed() const -> bool;

	private:
		friend class MouseDevice;

		unsigned int _lastEventIndex = 0u;
		Array<bool, static_cast<int>(MouseButton::Count)> _consumedButtons;
		bool _consumedMove;
		bool _consumedScroll;

		MouseDeviceState _state;
	};

	class MouseDevice
	{
	public:
		void Setup(Application& application);
		void Update(MouseState& state);

		auto Cursor() const -> MouseCursor;
		void SetCursor(MouseCursor cursor);

		auto DoubleClickDelay() const -> Time;

	private:
		std::unique_ptr<MouseInput> _input;
		MouseCursor _cursor = MouseCursor::Arrow;

		auto CreateInput() -> std::unique_ptr<MouseInput>;
	};
}

inline
auto Pargon::MouseState::PreviousMouseX() const -> float
{
	return _consumedMove ? _state.X : _state.PreviousX;
}

inline
auto Pargon::MouseState::PreviousMouseY() const -> float
{
	return _consumedMove ? _state.Y : _state.PreviousY;
}

inline
auto Pargon::MouseState::MouseX() const -> float
{
	return _state.X;
}

inline
auto Pargon::MouseState::MouseY() const -> float
{
	return _state.Y;
}

inline
auto Pargon::MouseState::PreviousScrollX() const -> float
{
	return _consumedScroll ? _state.ScrollX : _state.PreviousScrollX;
}

inline
auto Pargon::MouseState::PreviousScrollY() const -> float
{
	return _consumedScroll ? _state.ScrollY : _state.PreviousScrollY;
}

inline
auto Pargon::MouseState::ScrollX() const -> float
{
	return _state.ScrollX;
}

inline
auto Pargon::MouseState::ScrollY() const -> float
{
	return _state.ScrollY;
}

inline
auto Pargon::MouseState::IsButtonDown(MouseButton button) const -> bool
{
	return _state.Down.Item(static_cast<int>(button));
}

inline
auto Pargon::MouseState::WasButtonJustPressed(MouseButton button) const -> bool
{
	return WasButtonConsumed(button) ? false : _state.Pressed.Item(static_cast<int>(button));
}

inline
auto Pargon::MouseState::WasButtonJustReleased(MouseButton button) const -> bool
{
	return WasButtonConsumed(button) ? false : _state.Released.Item(static_cast<int>(button));
}

inline
void Pargon::MouseState::ConsumeAll()
{
	for (auto& consumedButton : _consumedButtons)
		consumedButton = true;

	_consumedMove = true;
	_consumedScroll = true;
}

inline
void Pargon::MouseState::ConsumeButton(MouseButton button)
{
	_consumedButtons.SetItem(static_cast<int>(button), true);
}

inline
void Pargon::MouseState::ConsumeMove()
{
	_consumedMove = true;
}

inline
void Pargon::MouseState::ConsumeScroll()
{
	_consumedScroll = true;
}

inline
auto Pargon::MouseState::WasButtonConsumed(MouseButton button) const -> bool
{
	return _consumedButtons.Item(static_cast<int>(button));
}

inline
auto Pargon::MouseState::WasMoveConsumed() const -> bool
{
	return _consumedMove;
}

inline
auto Pargon::MouseState::WasScrollConsumed() const -> bool
{
	return _consumedScroll;
}

inline
auto Pargon::MouseDevice::Cursor() const -> MouseCursor
{
	return _cursor;
}
