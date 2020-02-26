#pragma once

#include "Pargon/Containers/Array.h"
#include "Pargon/Math/Vector.h"

namespace Pargon
{
	class StringReader;
	class StringWriter;
	class ControllerInput;
	class ControllerState;

	enum class ControllerAxis
	{
		LeftTrigger,
		LeftStickHorizontal,
		LeftStickVertical,
		RightTrigger,
		RightStickHorizontal,
		RightStickVertical
	};

	enum class ControllerButton
	{
		Left, Right, Up, Down,
		A, B, X, Y,
		Start, Back,
		LeftBumper, RightBumper,
		LeftStick, RightStick,
		LeftTrigger, RightTrigger,
		LeftStickLeft, LeftStickRight, LeftStickUp, LeftStickDown,
		RightStickLeft, RightStickRight, RightStickUp, RightStickDown,
		Unknown, Count
	};

	struct ControllerMask
	{
	public:
		struct Iterator
		{
		public:
			auto operator*() -> int;
			auto operator!=(const Iterator& other) -> bool;
			auto operator++() -> Iterator&;

		private:
			friend struct ControllerMask;

			Iterator(int bit, int mask);

			int _bit;
			int _mask;
		};

		static auto All() -> ControllerMask;
		static auto None() -> ControllerMask;

		ControllerMask(bool first, bool second, bool third, bool fourth);

		auto begin() -> Iterator;
		auto end() -> Iterator;

		void Enable(int index);
		void Disable(int index);

	private:
		int _mask;
	};

	struct RumbleState
	{
		float HighFrequencyRumble = 0.0f;
		float LowFrequencyRumble = 0.0f;
	};

	struct ControllerDeviceState
	{
		bool PreviousConnected = false;
		bool Connected = false;
		float PreviousLeftTrigger = 0.0f;
		float PreviousRightTrigger = 0.0f;
		float LeftTrigger = 0.0f;
		float RightTrigger = 0.0f;
		Vector2 PreviousLeftStick;
		Vector2 PreviousRightStick;
		Vector2 LeftStick;
		Vector2 RightStick;
		Array<bool, static_cast<int>(ControllerButton::Count)> Down;
		Array<bool, static_cast<int>(ControllerButton::Count)> Pressed;
		Array<bool, static_cast<int>(ControllerButton::Count)> Released;
	};

	struct ControllerSettings
	{
		float StickDeadzone = 0.0f;
		float StickPressThreshold = 0.5f;
		float TriggerDeadzone = 0.0f;
		float TriggerPressThreshold = 0.2f;
	};

	class ControllerDevice
	{
	public:
		static constexpr int ControllerCount = 4;

		void Setup(ControllerSettings&& settings);
		void Update(ControllerState& state);

		void SetRumble(int index, float highFrequency, float lowFrequency);
		void UpdateRumble(float elapsed);

	private:
		ControllerSettings _settings;

		std::unique_ptr<ControllerInput> _input;

		Array<RumbleState, ControllerCount> _rumble;

		auto CreateInput() -> std::unique_ptr<ControllerInput>;
	};

	class ControllerState
	{
	public:
		auto IsControllerConnected(int index) const -> bool;
		auto WasControllerJustConnected(int index) const -> bool;
		auto WasControllerJustDisconnected(int index) const -> bool;
		auto LeftTrigger(int index) const -> float;
		auto RightTrigger(int index) const -> float;
		auto PreviousLeftTrigger(int index) const -> float;
		auto PreviousRightTrigger(int index) const -> float;
		auto LeftStick(int index) const -> Vector2;
		auto RightStick(int index) const -> Vector2;
		auto PreviousLeftStick(int index) const -> Vector2;
		auto PreviousRightStick(int index) const -> Vector2;
		auto IsButtonDown(int index, ControllerButton button) const -> bool;
		auto WasButtonJustPressed(int index, ControllerButton button) const -> bool;
		auto WasButtonJustReleased(int index, ControllerButton button) const -> bool;

		void ConsumeAll();
		void ConsumeAll(int index);
		void ConsumeLeftStick(int index);
		void ConsumeRightStick(int index);
		void ConsumeLeftTrigger(int index);
		void ConsumeRightTrigger(int index);
		void ConsumeButton(int index, ControllerButton button);

		auto WasLeftStickConsumed(int index) const -> bool;
		auto WasRightStickConsumed(int index) const -> bool;
		auto WasLeftTriggerConsumed(int index) const -> bool;
		auto WasRightTriggerConsumed(int index) const -> bool;
		auto WasButtonConsumed(int index, ControllerButton button) const -> bool;

	private:
		friend class ControllerDevice;

		int _lastEventIndex = 0;

		Array<bool, ControllerDevice::ControllerCount> _consumedLeftStick;
		Array<bool, ControllerDevice::ControllerCount> _consumedRightStick;
		Array<bool, ControllerDevice::ControllerCount> _consumedLeftTrigger;
		Array<bool, ControllerDevice::ControllerCount> _consumedRightTrigger;
		Array<Array<bool, static_cast<int>(ControllerButton::Count)>, ControllerDevice::ControllerCount> _consumedButtons;

		Array<ControllerDeviceState, ControllerDevice::ControllerCount> _states;
	};
}

inline
auto Pargon::ControllerMask::Iterator::operator*() -> int
{
	return _bit;
}

inline
auto Pargon::ControllerMask::Iterator::operator!=(const Iterator& other) -> bool
{
	return _bit != other._bit;
}

inline
auto Pargon::ControllerMask::Iterator::operator++() -> Iterator&
{
	while (_bit < 4 && (_mask & _bit) == 0) ++_bit; return *this;
}

inline
Pargon::ControllerMask::Iterator::Iterator(int bit, int mask) :
	_bit(bit), _mask(mask)
{
}

inline
auto Pargon::ControllerMask::All() -> ControllerMask
{
	return { true, true, true, true };
}

inline
auto Pargon::ControllerMask::None() -> ControllerMask
{
	return { false, false, false, false };
}

inline
Pargon::ControllerMask::ControllerMask(bool first, bool second, bool third, bool fourth) :
	_mask(0)
{
	if (first) _mask |= 0x1;
	if (second) _mask |= 0x2;
	if (third) _mask |= 0x4;
	if (fourth) _mask |= 0x8;
}

inline
auto Pargon::ControllerMask::begin() -> Iterator
{
	return { 0, _mask };
}

inline
auto Pargon::ControllerMask::end() -> Iterator
{
	return { 4, _mask };
}

inline
void Pargon::ControllerMask::Enable(int index)
{
	_mask |= (1 << index);
}

inline
void Pargon::ControllerMask::Disable(int index)
{
	_mask &= ~(1 << index);
}

inline
auto Pargon::ControllerState::WasControllerJustConnected(int index) const -> bool
{
	return _states.Item(index).Connected && !_states.Item(index).PreviousConnected;
}

inline
auto Pargon::ControllerState::WasControllerJustDisconnected(int index) const -> bool
{
	return !_states.Item(index).Connected && _states.Item(index).PreviousConnected;
}

inline
auto Pargon::ControllerState::IsControllerConnected(int index) const -> bool
{
	return _states.Item(index).Connected;
}

inline
auto Pargon::ControllerState::LeftTrigger(int index) const -> float
{
	return _states.Item(index).LeftTrigger;
}

inline
auto Pargon::ControllerState::RightTrigger(int index) const -> float
{
	return _states.Item(index).RightTrigger;
}

inline
auto Pargon::ControllerState::PreviousLeftTrigger(int index) const -> float
{
	return WasLeftTriggerConsumed(index) ? _states.Item(index).LeftTrigger : _states.Item(index).PreviousLeftTrigger;
}

inline
auto Pargon::ControllerState::PreviousRightTrigger(int index) const -> float
{
	return WasRightTriggerConsumed(index) ? _states.Item(index).RightTrigger : _states.Item(index).PreviousRightTrigger;
}

inline
auto Pargon::ControllerState::LeftStick(int index) const -> Vector2
{
	return _states.Item(index).LeftStick;
}

inline
auto Pargon::ControllerState::RightStick(int index) const -> Vector2
{
	return _states.Item(index).RightStick;
}

inline
auto Pargon::ControllerState::PreviousLeftStick(int index) const -> Vector2
{
	return WasLeftStickConsumed(index) ? _states.Item(index).LeftStick : _states.Item(index).PreviousLeftStick;
}

inline
auto Pargon::ControllerState::PreviousRightStick(int index) const -> Vector2
{
	return WasRightStickConsumed(index) ? _states.Item(index).RightStick : _states.Item(index).PreviousRightStick;
}

inline
auto Pargon::ControllerState::IsButtonDown(int index, ControllerButton button) const -> bool
{
	return _states.Item(index).Down.Item(static_cast<int>(button));
}

inline
auto Pargon::ControllerState::WasButtonJustPressed(int index, ControllerButton button) const -> bool
{
	return WasButtonConsumed(index, button) ? false : _states.Item(index).Pressed.Item(static_cast<int>(button));
}

inline
auto Pargon::ControllerState::WasButtonJustReleased(int index, ControllerButton button) const -> bool
{
	return WasButtonConsumed(index, button) ? false : _states.Item(index).Released.Item(static_cast<int>(button));
}

inline
void Pargon::ControllerState::ConsumeAll()
{
	for (auto i = 0; i < ControllerDevice::ControllerCount; ++i)
		ConsumeAll(i);
}

inline
void Pargon::ControllerState::ConsumeAll(int index)
{
	_consumedLeftStick.SetItem(index, true);
	_consumedRightStick.SetItem(index, true);
	_consumedLeftTrigger.SetItem(index, true);
	_consumedRightTrigger.SetItem(index, true);

	for (auto& consumedButton : _consumedButtons.Item(index))
		consumedButton = true;
}

inline
void Pargon::ControllerState::ConsumeLeftStick(int index)
{
	_consumedLeftStick.SetItem(index, true);
}

inline
void Pargon::ControllerState::ConsumeRightStick(int index)
{
	_consumedRightStick.SetItem(index, true);
}

inline
void Pargon::ControllerState::ConsumeLeftTrigger(int index)
{
	_consumedLeftTrigger.SetItem(index, true);
}

inline
void Pargon::ControllerState::ConsumeRightTrigger(int index)
{
	_consumedRightTrigger.SetItem(index, true);
}

inline
void Pargon::ControllerState::ConsumeButton(int index, ControllerButton button)
{
	_consumedButtons.Item(index).SetItem(static_cast<int>(button), true);
}

inline
auto Pargon::ControllerState::WasLeftStickConsumed(int index) const -> bool
{
	return _consumedLeftStick.Item(index);
}

inline
auto Pargon::ControllerState::WasRightStickConsumed(int index) const -> bool
{
	return _consumedRightStick.Item(index);
}

inline
auto Pargon::ControllerState::WasLeftTriggerConsumed(int index) const -> bool
{
	return _consumedLeftTrigger.Item(index);
}

inline
auto Pargon::ControllerState::WasRightTriggerConsumed(int index) const -> bool
{
	return _consumedRightTrigger.Item(index);
}

inline
auto Pargon::ControllerState::WasButtonConsumed(int index, ControllerButton button) const -> bool
{
	return _consumedButtons.Item(index).Item(static_cast<int>(button));
}
