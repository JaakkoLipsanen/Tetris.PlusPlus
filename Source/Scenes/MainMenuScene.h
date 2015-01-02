#pragma once
#include <Scenes/FadeableScene.h>

class MainMenuScene : public FadeableScene
{
public:
	explicit MainMenuScene();
	~MainMenuScene();

protected:
	void OnEntering() override;
	void Update() override;
	void Render() override;

private:
	struct Impl;
	std::unique_ptr<Impl> _pImpl;
};