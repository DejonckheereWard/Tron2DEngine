#include <SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>


bool Engine::InputManager::ProcessInput()
{
	//ZeroMemory

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		
		// Imgui
		ImGui_ImplSDL2_ProcessEvent(&e);

		// Fixes window not closing when clicking on the X
		if(e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)
			return false;
	}

	return true;
}
