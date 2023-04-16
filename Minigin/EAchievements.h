#pragma once


#include "steam_api.h"
#include "Achievement_t.h"
#include "CSteamAchievements.h"
#include "Singleton.h"
#include "Observer.h"
#include "GameObject.h"
#include "ScoreComponent.h"

// Defining our achievements
enum EAchievements
{
	ACH_WIN_ONE_GAME = 0,
	ACH_WIN_100_GAMES = 1,
	ACH_TRAVEL_FAR_ACCUM = 2,
	ACH_TRAVEL_FAR_SINGLE = 3,
};

// Achievement array which will hold data about the achievements and their state
Achievement_t g_Achievements[] =
{
	_ACH_ID(ACH_WIN_ONE_GAME, "Winner"),
	_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
	_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
	_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter"),
};

// Global access to Achievements object
Engine::CSteamAchievements* g_SteamAchievements = NULL;

class AchievementHandler final: public Engine::Singleton<AchievementHandler>, public Engine::Observer
{
public:
	void OnNotify(Engine::GameObject* pGameObject, const std::string& eventName) override
	{
		if(eventName == "ScoreChanged")
		{
			if(auto component = pGameObject->GetComponent<ScoreComponent>())
			{
				int score{ component->GetScore() };
				if(score >= 500)
				{
					g_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
				}
			}
		}
	};

private:
	friend class Singleton<AchievementHandler>;
	AchievementHandler() = default;
};