#include <Scenes/GameScene.h>
#include <Scenes/GameOverScene.h>
#include <Scenes/MainMenuScene.h>

#include <Engine/Game.h>
#include <Input/Input.h>

GameScene::GameScene() :
	FadeableScene(0.25f, Color::White), _level(), _levelRenderer(_level), _levelController(_level)
{
	_level.GameOver += [this](int finalScore)
	{
		this->ChangeScene(std::unique_ptr<Scene>(new GameOverScene(finalScore)));
	};
}

void GameScene::OnEntering()
{
	_levelRenderer.LoadContent(this->GetGame().GetGraphicsContext());
}

void GameScene::Update()
{
	_levelController.Control();
	_level.Update();
	_levelRenderer.Update();

	if (Input::IsNewKeyPress(KeyCode::Escape))
	{
		this->ChangeScene(std::unique_ptr<Scene>(new MainMenuScene()));
	}
}

void GameScene::Render()
{
	_levelRenderer.Render();
}
