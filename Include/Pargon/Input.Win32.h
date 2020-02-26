#pragma once

#include "Pargon/Input.h"
#include <memory>

namespace Pargon
{
	auto CreateWin32KeyboardInput() -> std::unique_ptr<KeyboardInput>;
	auto CreateWin32MouseInput()->std::unique_ptr<MouseInput>;
}
