#pragma once
#include "Scene.h"
#include <map>
#include <cmath>
#include <thread>

class TrashTheCacheScene
{
public:
	TrashTheCacheScene() {};

	// Inherited via Scene
	virtual void OnCreate() ;
	virtual void OnImGui() ;



private:
	struct Transform
	{
		float matrix[16] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	};

	class GameObject3D
	{
	public:
		Transform transform{};
		int ID{};
	};

	class GameObject3DAlt
	{
	public:
		// Unique ptr without custom delete has no size overhead compared to raw pointer
		static_assert(sizeof(std::unique_ptr<Transform>) == (sizeof(Transform*)));

		// Storing the transform as a pointer, meaning its stored somewhere else in memory and only the address remains here
		// This causes the caching to work better since more IDs can fit in the same cacheline, before the transform 
		// (that filled the whole cacheline) caused every new gameobject to have its id on a new cacheline, increasing cache misses
		std::unique_ptr<Transform> transform = std::make_unique<Transform>();
		int ID{};
	};


	int m_BufferSize{}; // Size of the array to be measured, set in oncreate for runtime 32/64 bit check
	const int m_MaxStepSize{ 1024 };
	
	int m_NrOfSamplesExercise01{ 10 };  // Changeable in gui
	int m_NrOfSamplesExercise02{ 10 };  // Changeable in gui

	// TrashTheCache
	void Exercise01();
	void Exercise02A();
	void Exercise02B();

	// Data from all exercises
	bool m_LoadingExercise01{ false };
	std::vector<float> m_Exercise01XData{};
	std::vector<float> m_Exercise01YData{};

	bool m_LoadingExercise02A{ false };
	std::vector<float> m_Exercise02AXData{};
	std::vector<float> m_Exercise02AYData{};	

	bool m_LoadingExercise02B{ false };
	std::vector<float> m_Exercise02BXData{};
	std::vector<float> m_Exercise02BYData{};

};