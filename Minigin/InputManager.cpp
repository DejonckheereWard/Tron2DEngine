#include <SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>


bool Engine::InputManager::ProcessInput()
{
	//ZeroMemory
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		if(e.type == SDL_QUIT) {
			return false;
		}

		// Imgui input handling
		ImGui_ImplSDL2_ProcessEvent(&e);

		// Fixes window not closing when clicking on the X
		if(e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)
			return false;
	}

	// OUTSIDE SDL POLL EVENT! (otherwise only update when mouse or keyboard key is pressed:( )
	HandleKeyboardInput();
	HandleControllerInput();

	return true;
}

unsigned int Engine::InputManager::AddController()
{
	// Add a default controller
	auto controllerIdx = static_cast<unsigned int>(m_Controllers.size());
	m_Controllers.push_back(std::make_unique<XController>(controllerIdx));
	return controllerIdx;
}

Engine::InputManager::InputManager():
	m_Controllers{}
{
}

Engine::InputManager::~InputManager() {}

void Engine::InputManager::HandleControllerInput()
{
	// Update every controller
	for(auto& controller : m_Controllers)
	{
		controller->Update();
	}

	// Loop over every input, commands mapping
	for(auto& controllerMapping : m_ControllerCommands)
	{
		// Check if the mapping should be executed
		// First.second.second is the buttonstate
		switch(controllerMapping.first.second.second)
		{
			// first.first is the controller index
			// First.second.first is the key
			case InputState::OnPress:
				if(m_Controllers[controllerMapping.first.first]->IsPressedThisFrame(controllerMapping.first.second.first))
				{
					// Execute every command in the mapping (vector of commands)
					for(auto& command : controllerMapping.second) { command->Execute(); }
				}
				break;
			case InputState::OnRelease:
				if(m_Controllers[controllerMapping.first.first]->IsReleasedThisFrame(controllerMapping.first.second.first))
				{
					// Execute every command in the mapping (vector of commands)
					for(auto& command : controllerMapping.second) { command->Execute(); }
				}
				break;
			case InputState::Pressed:
				if(m_Controllers[controllerMapping.first.first]->IsPressed(controllerMapping.first.second.first))
				{
					// Execute every command in the mapping (vector of commands)
					for(auto& command : controllerMapping.second) { command->Execute(); }
				}
				break;
			default:
				break;
		}
	}

	for(auto& controllerMapping : m_ControllerAxisCommands)
	{
		// Check if the mapping should be executed
		// first.first is the controller index
		// First.second.first is the key
		const float deadZone{ 0.2f };
		switch(static_cast<XController::ControllerAxis>(controllerMapping.first.second.first))
		{
			case XController::ControllerAxis::LeftThumbX:
			{

				glm::vec2 value{ m_Controllers[controllerMapping.first.first]->GetLeftThumb() };
				if(std::abs(value.x) > deadZone)
				{
					// Execute every command in the mapping (vector of commands)
					for(auto& command : controllerMapping.second) { command->Execute(value.x); }
				}
				break;
			}
			case XController::ControllerAxis::LeftThumbY:
			{

				glm::vec2 value{ m_Controllers[controllerMapping.first.first]->GetLeftThumb() };
				if(std::abs(value.y) > deadZone)
				{
					// Execute every command in the mapping (vector of commands)
					for(auto& command : controllerMapping.second) { command->Execute(value.y); }
				}
				break;
			}
			case XController::ControllerAxis::RightThumbX:
			{
				glm::vec2 value{ m_Controllers[controllerMapping.first.first]->GetRightThumb() };
				if(std::abs(value.x) > deadZone)
				{
					// Execute every command in the mapping (vector of commands)
					for(auto& command : controllerMapping.second) { command->Execute(value.x); }
				}
				break;
			}
			case XController::ControllerAxis::RightThumbY:
			{
				glm::vec2 value{ m_Controllers[controllerMapping.first.first]->GetRightThumb() };
				if(std::abs(value.y) > deadZone)
				{
					// Execute every command in the mapping (vector of commands)
					for(auto& command : controllerMapping.second) { command->Execute(value.y); }
				}
				break;
			}
			case XController::ControllerAxis::LeftTrigger:
			{
				float value{ m_Controllers[controllerMapping.first.first]->GetLeftTrigger() };
				if(value > deadZone)
				{
					// Execute every command in the mapping (vector of commands)
					for(auto& command : controllerMapping.second) { command->Execute(value); }
				}
				break;
			}
			case XController::ControllerAxis::RightTrigger:
			{
				float value{ m_Controllers[controllerMapping.first.first]->GetRightTrigger() };
				if(value > deadZone)
				{
					// Execute every command in the mapping (vector of commands)
					for(auto& command : controllerMapping.second) { command->Execute(value); }
				}
				break;
			}
			default:
				break;
		}
	}
}

void Engine::InputManager::HandleKeyboardInput()
{
	// Update the keyboard
	m_Keyboard->Update();

	for(auto& mapping : m_KeyboardCommands)
	{
		// Check press or release
		switch(mapping.first.second)
		{
			case InputState::OnPress:
				if(m_Keyboard->IsPressedThisFrame(mapping.first.first))
				{
					// Execute every command in the mapping (vector of commands)
					for(auto& command : mapping.second) { command->Execute(); }
				}
				break;
			case InputState::OnRelease:
				if(m_Keyboard->IsReleasedThisFrame(mapping.first.first))
				{
					// Execute every command in the mapping (vector of commands)
					for(auto& command : mapping.second) { command->Execute(); }
				}
				break;
			case InputState::Pressed:
				if(m_Keyboard->IsPressed(mapping.first.first))
				{
					// Execute every command in the mapping (vector of commands)
					for(auto& command : mapping.second) { command->Execute(); }
				}
				break;

			default:
				assert("Unknown InputState! Shouldnt happen!");
				break;
		}
	}
	for(auto& mapping : m_KeyboardAxisCommands)
	{
		if(m_Keyboard->IsPressed(mapping.first.first))
		{
			// Execute every command in the mapping (vector of commands)
			for(auto& command : mapping.second) 
			{ 
				command->Execute(1.0f); 
			}
		}
	}
}
