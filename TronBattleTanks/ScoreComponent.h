#pragma once


#include "BaseComponent.h"
#include "Subject.h"

class ScoreComponent final: public Engine::BaseComponent
{
public:
	ScoreComponent(Engine::GameObject* parent);
	~ScoreComponent() = default;

	// Inherited via BaseComponent
	virtual void Init() override;

	virtual void Update() override {};

	virtual void Render() const override {};

	int GetScore() const;
	void SetScore(int score);
	void AddScore(int score);
	void SubtractScore(int score);

	Engine::Subject* GetSubject() { return m_pSubject.get(); };

private:
	std::unique_ptr<Engine::Subject> m_pSubject{};

	int m_Score{ 0 };



};

