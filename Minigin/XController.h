#pragma once

#include <memory>
#include <glm/vec2.hpp>

namespace Engine
{
	// Xbox / Ps gamepad
	class XController final
	{
	public:
		enum class ControllerButton
		{
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008,
			Start = 0x0010,
			Back = 0x0020,
			LeftThumb = 0x0040,
			RightThumb = 0x0080,
			LeftShoulder = 0x0100,
			RightShoulder = 0x0200,
			ButtonA = 0x1000,
			ButtonB = 0x2000,
			ButtonX = 0x4000,
			ButtonY = 0x8000
		};
		
		enum class ControllerAxis
		{
			LeftThumbX = 0,
			LeftThumbY = 1,
			LeftTrigger = 2,
			RightTrigger = 3,
			RightThumbX = 4,
			RightThumbY = 5,
			LeftThumbXY = 6,
			RightThumbXY = 7

		};

		explicit XController(unsigned int controllerIndex);
		~XController();

		void Update();
		bool IsPressedThisFrame(ControllerButton button) const;  // flank detection
		bool IsReleasedThisFrame(ControllerButton button) const;  // flank detection
		bool IsPressed(ControllerButton button) const;

		glm::vec2 GetLeftThumb() const; // -1 - 1
		glm::vec2  GetRightThumb() const; // -1 - 1

		float GetLeftTrigger() const; // 0 - 1
		float GetRightTrigger() const; // 0 - 1


	private:
		class XControllerImpl;
		std::unique_ptr<XControllerImpl> m_ControllerImpl;
	};
}

