#pragma once
#include "Singleton.h"
#include <map>
#include <vector>
#include "XController.h"
#include "Keyboard.h"
#include "Command.h"

namespace Engine
{
	enum class InputState
	{
		OnPress,
		OnRelease,
		Pressed
	};

	template <typename T>
	concept IsCommand = std::is_base_of<Command, T>::value;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		void AddAction(unsigned int controllerIndex, XController::ControllerButton button, InputState btnState, std::unique_ptr<Command> command);
		
		void AddAction(SDL_Scancode key, InputState keySate, std::unique_ptr<Command> command);

		void AddAxisMapping(unsigned int controllerIndex, XController::ControllerAxis axis, std::unique_ptr<Command> command);

		void AddAxisMapping(SDL_Scancode key, std::unique_ptr<Command> command);

		unsigned int AddController();  // Returns index of the added controller
		//void AddCommand(SDL_KeyCode keyCode, SDL_Key)

	private:
		friend class Singleton<InputManager>;
		InputManager();
		virtual ~InputManager();
		void HandleControllerInput();
		void HandleKeyboardInput();
		
		using ControllerInput = std::pair<XController::ControllerButton, InputState>;
		using ControllerKey = std::pair<unsigned int, ControllerInput>;
		using ControllerCommandsMap = std::map<ControllerKey, std::vector<std::unique_ptr<Command>>>;
		using ControllerAxisCommandsMap = std::map<ControllerKey, std::vector<std::unique_ptr<Command>>>;

		using KeyboardInput = std::pair<SDL_Scancode, InputState>;
		using KeyboardCommandsMap = std::map<KeyboardInput, std::vector<std::unique_ptr<Command>>>;
		using KeyboardAxisCommandsMap = std::map<KeyboardInput, std::vector<std::unique_ptr<Command>>>;

		ControllerCommandsMap m_ControllerCommands{};
		ControllerAxisCommandsMap m_ControllerAxisCommands{};
		KeyboardCommandsMap m_KeyboardCommands{};
		KeyboardAxisCommandsMap m_KeyboardAxisCommands{};

		std::vector<std::unique_ptr<XController>> m_Controllers{};
		std::unique_ptr<Keyboard> m_Keyboard{ std::make_unique<Keyboard>() };
	
	};
}
