#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Keyboard.h"
#include <vector>
#include <SDL.h>

class Engine::Keyboard::KeyboardImpl
{
public:
	KeyboardImpl() :
		m_CurrentState(SDL_NUM_SCANCODES),
		m_PreviousState(SDL_NUM_SCANCODES),
		m_ButtonsPressedThisFrame(SDL_NUM_SCANCODES),
		m_ButtonsReleasedThisFrame(SDL_NUM_SCANCODES)
	{
		for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
		{
			m_CurrentState[i] = 0;
			m_PreviousState[i] = 0;
			m_ButtonsPressedThisFrame[i] = 0;
			m_ButtonsReleasedThisFrame[i] = 0;
		} 
	}

	void Update()
	{
		SDL_PumpEvents();  // Updates keyboard state

		m_PreviousState = m_CurrentState;

		int numKeys{};
		const Uint8* state = SDL_GetKeyboardState(&numKeys);

		for(int i = 0; i < numKeys; ++i)
		{
			m_CurrentState[i] = state[i];
		}


	}

	bool IsPressedThisFrame(SDL_Scancode key)
	{
		if(m_CurrentState[key] && !m_PreviousState[key])
		{
			return true;
		}
		return false;
	}

	bool IsReleasedThisFrame(SDL_Scancode key)
	{
		if(!m_CurrentState[key] && m_PreviousState[key])
		{
			return true;
		}
		return false;
	}

	bool IsPressed(SDL_Scancode key)
	{
		return m_CurrentState[key];
	}

private:
	std::vector<Uint8> m_CurrentState{};
	std::vector<Uint8> m_PreviousState{};

	std::vector<Uint8> m_ButtonsPressedThisFrame{};
	std::vector<Uint8> m_ButtonsReleasedThisFrame{};

};

Engine::Keyboard::Keyboard():
	m_KeyboardImpl{ std::make_unique<KeyboardImpl>() }
{}

Engine::Keyboard::~Keyboard()
{}

void Engine::Keyboard::Update()
{
	m_KeyboardImpl->Update();
}

bool Engine::Keyboard::IsPressedThisFrame(unsigned int key) const
{
	return m_KeyboardImpl->IsPressedThisFrame(static_cast<SDL_Scancode>(key));
}

bool Engine::Keyboard::IsReleasedThisFrame(unsigned int  key) const
{
	return m_KeyboardImpl->IsReleasedThisFrame(static_cast<SDL_Scancode>(key));
}

bool Engine::Keyboard::IsPressed(unsigned int  key) const
{
	return m_KeyboardImpl->IsPressed(static_cast<SDL_Scancode>(key));
}
