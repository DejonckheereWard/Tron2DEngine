#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "GameTimer.h"
#include <chrono>

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

Engine::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

Engine::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void Engine::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& collisionManager = CollisionManager::GetInstance();
	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& gameTime = GameTimer::GetInstance();

	auto lastTime{ std::chrono::high_resolution_clock::now() };

	const int frameRateCap{ 144 };  // Max framerate we want
	const int minMsPerFrame{ 1000 / frameRateCap };  // Min time before we want to update a frame again. (*1000 to convert from Seconds to Millis)

	const float fixedTimeStep{ 1.0f / 30.0f }; 
	gameTime.SetFixedDeltaTime(fixedTimeStep);

	sceneManager.Init();
	
	float lagTime{};  // Catchup time if we are behind on updating
	// todo: this update loop could use some work.
	bool doContinue = true;
	while (doContinue)
	{
		const auto currentTime{ std::chrono::high_resolution_clock::now()};  // Total time elapsed since start of program
		const auto deltaTime{ std::chrono::duration<float>(currentTime - lastTime).count()};

		gameTime.UpdateDeltaTime(deltaTime);  // Updates deltaTime & totalTime
		doContinue = input.ProcessInput();

		lagTime += deltaTime;  // Add time passed since last frame to lagTime

		while (lagTime >= fixedTimeStep)  // While we are behind on updating
		{
			sceneManager.FixedUpdate();
			collisionManager.FixedUpdate();  // Update physics
			lagTime -= fixedTimeStep;  // Remove fixedTimeStep from lagTime
		}


		sceneManager.Update();
		renderer.Render();
		
		lastTime = currentTime;

		sceneManager.Cleanup();  // Cleanup all objects that are marked for deletion
		collisionManager.Cleanup();  // Cleanup all objects that are marked for deletion
		// Sleep time later (limit fps)
		const auto sleepTime = currentTime + std::chrono::milliseconds(minMsPerFrame) - std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(sleepTime);
	}

	sceneManager.DestroyAll();
}
