#include "Pargon/Application/Application.h"
#include "Win32/Win32KeyboardInput.h"
#include "Pargon/Input.Win32.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

using namespace Pargon;

auto Pargon::CreateWin32KeyboardInput() -> std::unique_ptr<KeyboardInput>
{
	return std::make_unique<Win32KeyboardInput>();
}

namespace
{
	auto KeyToWin32Key(Key key) -> int
	{
		if (key >= Key::Zero && key <= Key::Nine)
			return static_cast<int>(key) + 22;

		if (key >= Key::A && key <= Key::Z)
			return static_cast<int>(key) + 65;

		if (key >= Key::F1 && key <= Key::F12)
			return static_cast<int>(key) + 76;

		if (key >= Key::Number0 && key <= Key::Number9)
			return static_cast<int>(key) + 13;

		switch (key)
		{
			case Key::Escape: return 27;
			case Key::Tab: return 9;
			case Key::Space: return 32;
			case Key::Backspace: return 8;
			case Key::Enter: return 13;
			case Key::Delete: return 46;
			case Key::LeftShift: return 16;
			case Key::RightShift: return 161;
			case Key::LeftControl: return 17;
			case Key::RightControl: return 163;
			case Key::LeftCommand: return 91;
			case Key::RightCommand: return 92;
			case Key::LeftAlt: return 164;
			case Key::RightAlt: return 165;
			case Key::CapsLock: return 20;
			case Key::Tilde: return 192;
			case Key::Plus: return 187;
			case Key::Minus: return 189;
			case Key::LeftBracket: return 219;
			case Key::RightBracket: return 221;
			case Key::Slash: return 220;
			case Key::Backslash: return 191;
			case Key::Comma: return 188;
			case Key::Period: return 190;
			case Key::Colon: return 186;
			case Key::Quote: return 222;
			case Key::Left: return 37;
			case Key::Right: return 39;
			case Key::Up: return 38;
			case Key::Down: return 40;
			case Key::Home: return 36;
			case Key::End: return 35;
			case Key::PageUp: return 33;
			case Key::PageDown: return 34;
			case Key::NumberPeriod: return 110;
			case Key::NumberPlus: return 107;
			case Key::NumberMinus: return 109;
			case Key::NumberEquals: return 12;
			case Key::NumberDivide: return 111;
			case Key::NumberMultiply: return 106;
			default: return 0;
		}
	}

	auto Win32KeyToKey(unsigned int key) -> Key
	{
		if (key >= 48 && key <= 57)
			return static_cast<Key>(key - 22);

		if (key >= 65 && key <= 90)
			return static_cast<Key>(key - 65);

		if (key >= 112 && key <= 123)
			return static_cast<Key>(key - 76);

		if (key >= 124 && key <= 135)
			return static_cast<Key>(key - 24);

		if (key >= 96 && key <= 105)
			return static_cast<Key>(key - 12);

		switch (key)
		{
			case 27: return Key::Escape;
			case 9: return Key::Tab;
			case 32: return Key::Space;
			case 8: return Key::Backspace;
			case 13: return Key::Enter;
			case 46: return Key::Delete;
			case 16: return Key::LeftShift;
			case 160: return Key::LeftShift;
			case 161: return Key::RightShift;
			case 17: return Key::LeftControl;
			case 162: return Key::LeftControl;
			case 163: return Key::RightControl;
			case 91: return Key::LeftCommand;
			case 92: return Key::RightCommand;
			case 18: return Key::LeftAlt;
			case 164: return Key::LeftAlt;
			case 165: return Key::RightAlt;
			case 20: return Key::CapsLock;
			case 192: return Key::Tilde;
			case 187: return Key::Plus;
			case 189: return Key::Minus;
			case 219: return Key::LeftBracket;
			case 221: return Key::RightBracket;
			case 220: return Key::Slash;
			case 191: return Key::Backslash;
			case 188: return Key::Comma;
			case 190: return Key::Period;
			case 186: return Key::Colon;
			case 222: return Key::Quote;
			case 37: return Key::Left;
			case 39: return Key::Right;
			case 38: return Key::Up;
			case 40: return Key::Down;
			case 36: return Key::Home;
			case 35: return Key::End;
			case 33: return Key::PageUp;
			case 34: return Key::PageDown;
			case 110: return Key::NumberPeriod;
			case 107: return Key::NumberPlus;
			case 109: return Key::NumberMinus;
			case 12: return Key::NumberEquals;
			case 111: return Key::NumberDivide;
			case 106: return Key::NumberMultiply;
			case 94: return Key::Return;
			case 144: return Key::NumberLock;
			default: return Key::Unknown;
		}
	}

	auto Win32KeyToModifiers(int key) -> ModifierKeys
	{
		auto keys = ModifierKeys();

		if (key & MOD_CONTROL) keys.Control = true;
		if (key & MOD_ALT) keys.Alt = true;
		if (key & MOD_SHIFT) keys.Shift = true;
		if (key & MOD_WIN) keys.Command = true;

		return keys;
	}

	auto ModifiersToWin32Key(const ModifierKeys& modifiers) -> int
	{
		auto key = 0u;

		if (modifiers.Control) key |= MOD_CONTROL;
		if (modifiers.Alt) key |= MOD_ALT;
		if (modifiers.Shift) key |= MOD_SHIFT;
		if (modifiers.Command) key |= MOD_WIN;

		return key;
	}
}

auto ModifierKeys::CommandName() -> StringView
{
	return "Windows";
}

auto ModifierKeys::AltName() -> StringView
{
	return "Alt";
}

void Win32KeyboardInput::Setup(Application& application)
{
	_application = application.Data();

	_application->AddHandler(WM_SYSKEYDOWN, [=](WPARAM w, LPARAM l) { return KeyPressed(w, l); });
	_application->AddHandler(WM_KEYDOWN, [=](WPARAM w, LPARAM l) { return KeyPressed(w, l); });
	_application->AddHandler(WM_SYSKEYUP, [=](WPARAM w, LPARAM l) { return KeyReleased(w, l); });
	_application->AddHandler(WM_KEYUP, [=](WPARAM w, LPARAM l) { return KeyReleased(w, l); });
}

auto Win32KeyboardInput::Update(KeyboardDeviceState& state, int eventIndex) -> int
{
	state.CapsLock = GetKeyState(VK_CAPITAL);

	auto currentIndex = _events.CurrentIndex();

	for (auto i = 0; i < EnumNames<Key>.Count(); i++)
	{
		state.Pressed.SetItem(i, false);
		state.Released.SetItem(i, false);
	}

	while (eventIndex != currentIndex)
	{
		auto& event = _events.Event(eventIndex);
		auto key = Win32KeyToKey(static_cast<int>(event.Key));

		state.Down.SetItem(static_cast<int>(key), event.Pressed);

		if (event.Pressed)
			state.Pressed.SetItem(static_cast<int>(key), true);
		else
			state.Released.SetItem(static_cast<int>(key), true);

		eventIndex = _events.NextIndex(eventIndex);
	}

	return eventIndex;
}

void Win32KeyboardInput::ShowKeyboard()
{
}

void Win32KeyboardInput::HideKeyboard()
{
}

auto Win32KeyboardInput::KeyPressed(WPARAM w, LPARAM l) -> bool
{
	_events.AddEvent({ true, w });
	return true;
}

auto Win32KeyboardInput::KeyReleased(WPARAM w, LPARAM l) -> bool
{
	_events.AddEvent({ false, w });
	return true;
}
