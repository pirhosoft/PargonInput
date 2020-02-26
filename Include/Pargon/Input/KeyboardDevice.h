#pragma once

#include "Pargon/Containers/Array.h"
#include "Pargon/Containers/String.h"
#include "Pargon/Serialization/Serialization.h"

namespace Pargon
{
	class Application;
	class StringWriter;
	class StringReader;
	class KeyboardInput;

	enum class Key
	{
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		Escape, Tab, Space, Backspace, Enter, Delete, Return,
		LeftShift, RightShift, LeftControl, RightControl, LeftCommand, RightCommand, LeftAlt, RightAlt, CapsLock, Function,
		Tilde, Plus, Minus, LeftBracket, RightBracket, Slash, Backslash, Comma, Period, Colon, Quote,
		Left, Right, Up, Down, Home, End, PageUp, PageDown,
		Number0, Number1, Number2, Number3, Number4, Number5, Number6, Number7, Number8, Number9,
		NumberPeriod, NumberPlus, NumberMinus, NumberEquals, NumberDivide, NumberMultiply, NumberLock,
		F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
		Unknown, Count
	};

	template<> auto EnumNames<Key> = SetEnumNames
	(
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
		"Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine",
		"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
		"Escape", "Tab", "Space", "Backspace", "Enter", "Delete", "Return",
		"LeftShift", "RightShift", "LeftControl", "RightControl", "LeftCommand", "RightCommand", "LeftAlt", "RightAlt", "CapsLock", "Function",
		"Tilde", "Plus", "Minus", "LeftBracket", "RightBracket", "Slash", "Backslash", "Comma", "Period", "Colon", "Quote",
		"Left", "Right", "Up", "Down", "Home", "End", "PageUp", "PageDown",
		"Number0", "Number1", "Number2", "Number3", "Number4", "Number5", "Number6", "Number7", "Number8", "Number9",
		"NumberPeriod", "NumberPlus", "NumberMinus", "NumberEquals", "NumberDivide", "NumberMultiply", "NumberLock",
		"F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24",
		"Unknown"
	);

	auto KeyToChar(Key key, bool shift) -> char;

	struct ModifierKeys
	{
		static auto CommandName() -> StringView;
		static auto AltName() -> StringView;

		ModifierKeys() = default;
		ModifierKeys(bool control, bool alt, bool shift, bool command, bool capsLock);

		bool Control = false;
		bool Alt = false;
		bool Shift = false;
		bool Command = false;
		bool CapsLock = false;

		auto operator==(const ModifierKeys& other) -> bool;

		void ToString(StringWriter& writer) const;
		auto FromString(StringReader& reader) -> bool;
	};

	struct Hotkey
	{
		ModifierKeys Modifiers;
		Key Key;

		auto operator==(const Hotkey& other) -> bool;

		void ToString(StringWriter& writer) const;
		auto FromString(StringReader& reader) -> bool;
	};

	class KeyboardState;

	struct KeyboardDeviceState
	{
		bool CapsLock = false;

		Array<bool, static_cast<int>(Key::Count)> Down;
		Array<bool, static_cast<int>(Key::Count)> Pressed;
		Array<bool, static_cast<int>(Key::Count)> Released;
	};

	class KeyboardState
	{
	public:
		auto IsKeyDown(Key key) const -> bool;
		auto WasKeyJustPressed(Key key) const -> bool;
		auto WasKeyJustReleased(Key key) const -> bool;

		auto IsCapsLockOn() const -> bool;
		auto AreModifiersDown(ModifierKeys modifiers) const -> bool;

		auto IsHotkeyDown(Hotkey key) const -> bool;
		auto WasHotkeyJustPressed(Hotkey key) const -> bool;
		auto WasHotkeyJustReleased(Hotkey key) const -> bool;

		void ConsumeAll();
		void ConsumeKey(Key key);
		auto WasKeyConsumed(Key key) const -> bool;

	private:
		friend class KeyboardDevice;

		unsigned int _lastEventIndex = 0u;
		Array<bool, static_cast<int>(Key::Count)> _consumedKeys;

		KeyboardDeviceState _state;
	};

	class KeyboardDevice
	{
	public:
		void Setup(Application& application, std::unique_ptr<KeyboardInput>&& input);
		void Update(KeyboardState& state);
		void ShowKeyboard();
		void HideKeyboard();

	private:
		std::unique_ptr<KeyboardInput> _input;
	};
}

inline
auto Pargon::KeyboardState::IsKeyDown(Key key) const -> bool
{
	return _state.Down.Item(static_cast<int>(key));
}

inline
auto Pargon::KeyboardState::WasKeyJustPressed(Key key) const -> bool
{
	return WasKeyConsumed(key) ? false : _state.Pressed.Item(static_cast<int>(key));
}

inline
auto Pargon::KeyboardState::WasKeyJustReleased(Key key) const -> bool
{
	return WasKeyConsumed(key) ? false : _state.Released.Item(static_cast<int>(key));
}

inline
auto Pargon::KeyboardState::IsCapsLockOn() const -> bool
{
	return _state.CapsLock;
}

inline
auto Pargon::KeyboardState::AreModifiersDown(ModifierKeys modifiers) const -> bool
{
	if (modifiers.Shift && !_state.Down.Item(static_cast<int>(Key::LeftShift)) && !_state.Down.Item(static_cast<int>(Key::RightShift))) return false;
	if (modifiers.Command && !_state.Down.Item(static_cast<int>(Key::LeftCommand)) && !_state.Down.Item(static_cast<int>(Key::RightCommand))) return false;
	if (modifiers.Control && !_state.Down.Item(static_cast<int>(Key::LeftControl)) && !_state.Down.Item(static_cast<int>(Key::RightControl))) return false;
	if (modifiers.Alt && !_state.Down.Item(static_cast<int>(Key::LeftAlt)) && !_state.Down.Item(static_cast<int>(Key::RightAlt))) return false;

	return true;
}

inline
auto Pargon::KeyboardState::IsHotkeyDown(Hotkey key) const -> bool
{
	return AreModifiersDown(key.Modifiers) && IsKeyDown(key.Key);
}

inline
auto Pargon::KeyboardState::WasHotkeyJustPressed(Hotkey key) const -> bool
{
	return AreModifiersDown(key.Modifiers) && WasKeyJustPressed(key.Key);
}

inline
auto Pargon::KeyboardState::WasHotkeyJustReleased(Hotkey key) const -> bool
{
	return AreModifiersDown(key.Modifiers) && WasKeyJustReleased(key.Key);
}

inline
void Pargon::KeyboardState::ConsumeAll()
{
	for (auto& consumedKey : _consumedKeys)
		consumedKey = true;
}

inline
void Pargon::KeyboardState::ConsumeKey(Key key)
{
	_consumedKeys.SetItem(static_cast<int>(key), true);
}

inline
auto Pargon::KeyboardState::WasKeyConsumed(Key key) const -> bool
{
	return _consumedKeys.Item(static_cast<int>(key));
}
