#pragma once

#include "Pargon/Math/Rotation.h"
#include "Pargon/Math/Vector.h"

#include <memory>

namespace Pargon
{
	class MotionInput;

	struct MotionDeviceState
	{
		Vector3 Acceleration;
		Vector3 Gravity;

		Rotation Yaw;
		Rotation Pitch;
		Rotation Roll;
	};

	class MotionState
	{
	public:
		auto Acceleration() const -> Vector3;
		auto Gravity() const -> Vector3;
		auto Roll() const -> Rotation;
		auto Pitch() const -> Rotation;
		auto Yaw() const -> Rotation;

	private:
		friend class MotionDevice;

		MotionDeviceState _state;
	};

	class MotionDevice
	{
	public:
		void Setup();
		void Update(MotionState& state);

	private:
		std::unique_ptr<MotionInput> _input;

		auto CreateInput() -> std::unique_ptr<MotionInput>;
	};
}

inline
auto Pargon::MotionState::Acceleration() const -> Vector3
{
	return _state.Acceleration;
}

inline
auto Pargon::MotionState::Gravity() const -> Vector3
{
	return _state.Gravity;
}

inline
auto Pargon::MotionState::Roll() const -> Rotation
{
	return _state.Roll;
}

inline
auto Pargon::MotionState::Pitch() const -> Rotation
{
	return _state.Pitch;
}

inline
auto Pargon::MotionState::Yaw() const -> Rotation
{
	return _state.Yaw;
}
