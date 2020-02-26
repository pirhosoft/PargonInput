#pragma once

#include "Pargon/Input.h"
#include <memory>

namespace Pargon
{
	auto CreateXInputControllerInput() -> std::unique_ptr<ControllerInput>;
}
