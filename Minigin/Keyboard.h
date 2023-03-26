#pragma once
#include <memory>
#include <SDL.h>

namespace Engine
{
	class Keyboard final
	{
	public:
		explicit Keyboard();
		~Keyboard();

		void Update();
		bool IsPressedThisFrame(SDL_Scancode key) const;  // flank detection
		bool IsReleasedThisFrame(SDL_Scancode key) const;  // flank detection
		bool IsPressed(SDL_Scancode key) const;

	private:
		class KeyboardImpl;
		std::unique_ptr<KeyboardImpl> m_KeyboardImpl;



	};
}

