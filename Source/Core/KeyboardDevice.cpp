#include "Pargon/Input/KeyboardDevice.h"
#include "Pargon/Input/KeyboardInput.h"
#include "Pargon/Serialization/StringReader.h"
#include "Pargon/Serialization/StringWriter.h"

using namespace Pargon;

namespace
{
	auto _lowerCaseMap = "abcdefghijklmnopqrstuvwxyz0123456789\0\0\0\0\0\0\0\0\0\0\0\0\0\t \0\n\0\0\0\0\0\0\0\0\0\0\0\0`=-[]\\/,.;\'\0\0\0\0\0\0\0\0000123456789.+-=/*\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	auto _upperCaseMap = "ABCDEFGHIJKLMNOPQRSTUVWXYZ)!@#$%^&*(\0\0\0\0\0\0\0\0\0\0\0\0\0\t \0\n\0\0\0\0\0\0\0\0\0\0\0\0~+_{}|?<>:\"\0\0\0\0\0\0\0\0000123456789.+-=/*\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
}

auto Pargon::KeyToChar(Key key, bool shift) -> char
{
	return shift ? _upperCaseMap[static_cast<int>(key)] : _lowerCaseMap[static_cast<int>(key)];
}

ModifierKeys::ModifierKeys(bool control, bool alt, bool shift, bool command, bool capsLock) :
	Control(control),
	Alt(alt),
	Shift(shift),
	Command(command),
	CapsLock(capsLock)
{
}

auto ModifierKeys::operator==(const ModifierKeys& other) -> bool
{
	return Control == other.Control && Alt == other.Alt && Shift == other.Shift && Command == other.Command && CapsLock == other.CapsLock;
}

auto Hotkey::operator==(const Hotkey& other) -> bool
{
	return Modifiers == other.Modifiers && Key == other.Key;
}

namespace
{
	auto ReadModifier(StringReader& reader, ModifierKeys& modifiers) -> bool
	{
		if (reader.AtEnd())
			return false;

		auto count = reader.AdvanceTo('+', true);
		if (count == -1)
		{
			count = reader.Remaining();
			reader.Advance(count);
		}

		auto name = reader.ViewPrevious(count);

		if (name == ModifierKeys::CommandName())
			modifiers.Command = true;
		else if (name == "Control")
			modifiers.Control = true;
		else if (name == ModifierKeys::AltName())
			modifiers.Alt = true;
		else if (name == "Shift")
			modifiers.Shift = true;
		else if (name == "CapsLock")
			modifiers.CapsLock = true;
		else
			return false;

		return reader.Advance(1);
	}

	auto WriteModifier(StringWriter& writer, bool& first, bool modifier, StringView name) -> bool
	{
		if (modifier)
		{
			if (first)
				first = false;
			else
				writer.Write('+');

			writer.Write(name, {});
		}

		return true;
	}
}

void ModifierKeys::ToString(StringWriter& writer) const
{
	auto first = true;
	WriteModifier(writer, first, Command, ModifierKeys::CommandName());
	WriteModifier(writer, first, Control, "Control");
	WriteModifier(writer, first, Alt, ModifierKeys::AltName());
	WriteModifier(writer, first, Shift, "Shift");
	WriteModifier(writer, first, CapsLock, "CapsLock");
}

auto ModifierKeys::FromString(StringReader& reader) -> bool
{
	while (ReadModifier(reader, *this));
	return true;
}

void Hotkey::ToString(StringWriter& writer) const
{
	//if (Modifiers.Control || Modifiers.Command || Modifiers.Alt || Modifiers.Shift)
	//	return writer.Format("{}+{}", Modifiers, Key);

	//return writer.Write(Key);
}

auto Hotkey::FromString(StringReader& reader) -> bool
{
	auto plus = IndexOf(reader.ViewRemaining(), '+');

	//if (plus == String::InvalidIndex)
	//	reader.Read(Key);

	return false;//reader.Parse("{}+{}", Modifiers, Key);
}

void KeyboardDevice::Setup(Application& application, std::unique_ptr<KeyboardInput>&& input)
{
	_input = std::move(input);
	_input->Setup(application);
}

void KeyboardDevice::Update(KeyboardState& state)
{
	state._lastEventIndex = _input->Update(state._state, state._lastEventIndex);

	for (auto& consumedKey : state._consumedKeys)
		consumedKey = false;
}

void KeyboardDevice::ShowKeyboard()
{
	_input->ShowKeyboard();
}

void KeyboardDevice::HideKeyboard()
{
	_input->HideKeyboard();
}
