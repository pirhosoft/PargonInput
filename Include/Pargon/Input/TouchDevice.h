#pragma once

#include "Pargon/Containers/Array.h"
#include "Pargon/Math/Vector.h"

namespace Pargon
{
	class Application;
	class TouchInput;
	class TouchState;

	struct Touch
	{
		Vector2 PreviousPosition;
		Vector2 Position;
		bool Down = false;
		bool Pressed = false;
		bool Released = false;
	};

	class TouchDevice
	{
	public:
		static constexpr int MaxTouches = 2u;

		void Setup(Application& application);
		void Update(TouchState& state);

	private:
		std::unique_ptr<TouchInput> _input;

		auto CreateInput() -> std::unique_ptr<TouchInput>;
	};

	struct TouchDeviceState
	{
		Array<Touch, TouchDevice::MaxTouches> Touches;
	};

	class TouchState
	{
	public:
		auto TouchPosition(int index) const -> Vector2;
		auto PreviousTouchPosition(int index) const -> Vector2;
		auto IsTouchActive(int index) const -> bool;
		auto WasJustTouched(int index) const -> bool;
		auto WasJustReleased(int index) const -> bool;

		void ConsumeAll();
		void ConsumeTouch(int index);
		auto WasTouchConsumed(int index) const -> bool;

	private:
		friend class TouchDevice;

		int _lastEventIndex = 0u;
		Array<bool, TouchDevice::MaxTouches> _consumedTouches;

		TouchDeviceState _state;
	};
}

inline
auto Pargon::TouchState::TouchPosition(int index) const -> Vector2
{
	return index < TouchDevice::MaxTouches ? _state.Touches.Item(index).Position : Vector2();
}

inline
auto Pargon::TouchState::PreviousTouchPosition(int index) const -> Vector2
{
	return index < TouchDevice::MaxTouches
		? (WasTouchConsumed(index) ? _state.Touches.Item(index).Position : _state.Touches.Item(index).PreviousPosition)
		: Vector2();
}

inline
auto Pargon::TouchState::IsTouchActive(int index) const -> bool
{
	return index < TouchDevice::MaxTouches ? _state.Touches.Item(index).Down : false;
}

inline
auto Pargon::TouchState::WasJustTouched(int index) const -> bool
{
	return index < TouchDevice::MaxTouches && !WasTouchConsumed(index) ? _state.Touches.Item(index).Pressed : false;
}

inline
auto Pargon::TouchState::WasJustReleased(int index) const -> bool
{
	return index < TouchDevice::MaxTouches && !WasTouchConsumed(index) ? _state.Touches.Item(index).Released : false;
}

inline
void Pargon::TouchState::ConsumeAll()
{
	for (auto& consumedTouch : _consumedTouches)
		consumedTouch = true;
}

inline
void Pargon::TouchState::ConsumeTouch(int index)
{
	_consumedTouches.SetItem(index, true);
}

inline
auto Pargon::TouchState::WasTouchConsumed(int index) const -> bool
{
	return _consumedTouches.Item(index);
}
