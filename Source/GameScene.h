#pragma once
#include <Engine/Scene.h>
#include <Level.h>
#include <LevelRenderer.h>
#include <LevelController.h>

class GameScene : public Scene
{
public:
	GameScene();

protected:
	void OnEntering() override;
	void Update() override;
	void Render() override;

private:
 // Level _level; (Level-> Board, Score, Time etc?)
 // LevelRenderer _renderer (LevelRenderer -> Score, Holding Block, BoardRenderer)
 //	LevelInput?

	Level _level;
	LevelRenderer _levelRenderer;
	LevelController _levelController;
};