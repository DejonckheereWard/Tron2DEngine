#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>

#include "XController.h"


// Implementation

class Engine::XController::XControllerImpl
{
public:
	XControllerImpl(unsigned int _controllerIndex):
		m_ControllerIndex{ _controllerIndex }
	{
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
	}

	void Update()
	{
		CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		XInputGetState(m_ControllerIndex, &m_CurrentState);

		int buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
	}

	bool IsPressedThisFrame(unsigned int button) const { return m_ButtonsPressedThisFrame & button; }
	bool IsReleasedThisFrame(unsigned int button) const { return m_ButtonsReleasedThisFrame & button; }
	bool IsPressed(unsigned int button) const { return m_CurrentState.Gamepad.wButtons & button; }

	glm::vec2 GetLeftThumb() const
	{
		glm::vec2 value{};
		// Division value from xinput documentation (max value is 32767)
		value.x = static_cast<float>(m_CurrentState.Gamepad.sThumbLX) / 32767.0f;
		value.y = static_cast<float>(m_CurrentState.Gamepad.sThumbLY) / 32767.0f;

		return value;
	}
	glm::vec2  GetRightThumb() const
	{
		glm::vec2 value{};
		// Division value from xinput documentation (max value is 32767)
		value.x = static_cast<float>(m_CurrentState.Gamepad.sThumbRX) / 32767.0f;
		value.y = static_cast<float>(m_CurrentState.Gamepad.sThumbRY) / 32767.0f;

		return value;
	}

	float GetLeftTrigger() const
	{
		// 255.0f is the max value (see documentation xinput)
		return static_cast<float>(m_CurrentState.Gamepad.bLeftTrigger) / 255.0f;
	}

	float GetRightTrigger() const
	{
		// 255.0f is the max value (see documentation xinput)
		return static_cast<float>(m_CurrentState.Gamepad.bRightTrigger) / 255.0f;
	}


private:
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};

	WORD m_ButtonsPressedThisFrame{};
	WORD m_ButtonsReleasedThisFrame{};

	unsigned int m_ControllerIndex{ 0 };
};

// Controller

Engine::XController::XController(unsigned int _controllerIndex):
	m_ControllerImpl{ std::make_unique<XControllerImpl>(_controllerIndex) }
{
}

Engine::XController::~XController()
{
}

void Engine::XController::Update()
{
	m_ControllerImpl->Update();
}

bool Engine::XController::IsPressedThisFrame(ControllerButton button) const
{
	return m_ControllerImpl->IsPressedThisFrame(static_cast<unsigned int>(button));
}

bool Engine::XController::IsReleasedThisFrame(ControllerButton button) const
{
	return m_ControllerImpl->IsReleasedThisFrame(static_cast<unsigned int>(button));
}

bool Engine::XController::IsPressed(ControllerButton button) const
{
	return m_ControllerImpl->IsPressed(static_cast<unsigned int>(button));
}

glm::vec2 Engine::XController::GetLeftThumb() const
{
	return m_ControllerImpl->GetLeftThumb();
}

glm::vec2 Engine::XController::GetRightThumb() const
{
	return m_ControllerImpl->GetRightThumb();
}

float Engine::XController::GetLeftTrigger() const
{
	return m_ControllerImpl->GetLeftTrigger();
}

float Engine::XController::GetRightTrigger() const
{
	return m_ControllerImpl->GetRightTrigger();
}
