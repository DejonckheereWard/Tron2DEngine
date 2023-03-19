#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"


#include "MainScene.h"
#include "TrashTheCacheScene.h"

void load()
{
	using namespace Engine;
	
	//SceneManager::GetInstance().AddScene(new MainScene());
	SceneManager::GetInstance().AddScene(new TrashTheCacheScene());

}

int main(int, char* []) {
	Engine::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}