#include <Scenes/GameScene.h>

#include <Engine/Game.h>
#include <Input/Input.h>
#include <Engine/ISceneManager.h>

GameScene::GameScene() :
	_level(), _levelRenderer(_level), _levelController(_level)
{
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

	if (Input::IsNewKeyPress(KeyCode::LeftControl))
	{
		this->GetSceneManager().LoadScene(std::unique_ptr<Scene>(new GameScene));
	}
}

void GameScene::Render()
{
	_levelRenderer.Render();
}
