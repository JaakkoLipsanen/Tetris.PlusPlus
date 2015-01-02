#pragma once
#include <Scenes/FadeableScene.h>
#include <memory>

class GameOverScene : public FadeableScene
{
public:
	explicit GameOverScene(int finalScore);
	~GameOverScene();

protected:
	void OnEntering() override;
	void Update() override;
	void Render() override;

private:
	struct Impl;
	std::unique_ptr<Impl> _pImpl;
};