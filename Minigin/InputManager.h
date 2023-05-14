#pragma once
#include "Singleton.h"
#include <map>
#include <vector>
#include "XController.h"
#include "Keyboard.h"
#include "Command.h"
#include <SDL.h>

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

		void AddAxisMapping(SDL_Scancode key, std::unique_ptr<Command> command, const glm::vec2& value = { 1.0f, 0.0f });

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

	//template<IsCommand T>
	//inline void InputManager::AddAction(unsigned int controllerIndex, XController::ControllerButton button, InputState btnState, GameObject* actor)
	//{
	//	auto input = ControllerInput(button, btnState);
	//	auto key = ControllerKey(controllerIndex, input);

	//	// Initialize vector of commands if no commands exist for this mapping yet.
	//	if(m_ControllerCommands.count(key) == 0)
	//	{
	//		m_ControllerCommands[key] = std::vector<std::unique_ptr<Command>>{};
	//	}

	//	m_ControllerCommands[key].push_back(std::make_unique<T>(actor));
	//}

	//template<IsCommand T>
	//inline void InputManager::AddAction(SDL_Scancode key, InputState keySate, GameObject* actor)
	//{
	//	auto input = KeyboardInput(key, keySate);

	//	// Initialize vector of commands if no commands exist for this mapping yet.
	//	if(m_KeyboardCommands.count(input) == 0)
	//	{
	//		m_KeyboardCommands[input] = std::vector<std::unique_ptr<Command>>{};
	//	}

	//	m_KeyboardCommands[input].push_back(std::make_unique<T>(actor));
	//}

	//template<IsAxisCommand T>
	//inline void InputManager::AddAxisMapping(unsigned int controllerIndex, XController::ControllerAxis axis, GameObject* actor)
	//{
	//	auto input = ControllerInput(static_cast<XController::ControllerButton>(axis), InputState::Pressed);
	//	auto key = ControllerKey(controllerIndex, input);

	//	// Initialize vector of commands if no commands exist for this mapping yet.
	//	if(m_ControllerCommands.count(key) == 0)
	//	{
	//		m_ControllerAxisCommands[key] = std::vector<std::unique_ptr<AxisCommand>>{};
	//	}

	//	m_ControllerAxisCommands[key].push_back(std::make_unique<T>(actor));
	//}

	//inline void InputManager::AddAction(SDL_Scancode key, InputState keySate, std::unique_ptr<Command> command)
	//{
	//}

	//inline void InputManager::AddAxisMapping(unsigned int controllerIndex, XController::ControllerAxis axis, std::unique_ptr<Command> command)
	//{
	//}

	//template<IsAxisCommand T>
	//inline void InputManager::AddAxisMapping(SDL_Scancode key, std::unique_ptr<Command> command)
	//{
	//	InputState state = InputState::Pressed;
	//	auto input = KeyboardInput(key, state);

	//	if(m_KeyboardAxisCommands.count(input) == 0)
	//	{
	//		m_KeyboardAxisCommands[input] = std::vector<std::unique_ptr<AxisCommand>>{};
	//	}

	//	m_KeyboardAxisCommands[input].push_back(std::make_unique<T>(actor, invertAxis));
	//}
}
