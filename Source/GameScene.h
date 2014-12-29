#pragma once
#include <Engine/Scene.h>

class GameScene : public Scene
{
protected:
	void OnEntering() override;
	void Update() override;
};
