#pragma once
#include <memory>

namespace Engine
{
	class Keyboard final
	{
	public:
		explicit Keyboard();
		~Keyboard();

		void Update();
		bool IsPressedThisFrame(unsigned int key) const;  // flank detection
		bool IsReleasedThisFrame(unsigned key) const;  // flank detection
		bool IsPressed(unsigned key) const;

	private:
		class KeyboardImpl;
		std::unique_ptr<KeyboardImpl> m_KeyboardImpl;



	};
}

