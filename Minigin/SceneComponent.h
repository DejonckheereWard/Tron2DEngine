#pragma once
#include "BaseComponent.h"
//#include <string>

namespace Engine
{
	class SceneComponent: public BaseComponent
	{
	public:
		SceneComponent(GameObject* pOwner);
		virtual ~SceneComponent();
		virtual void Init() override {};
		virtual void Update(float) override {};
		virtual void Render() const override {};

		void SetName(const std::string& name) { m_SceneName = name; };
		const std::string& GetName() { return m_SceneName; };

		


	private:
		std::string m_SceneName{};
		static unsigned int m_SceneCounter;
	};
}

