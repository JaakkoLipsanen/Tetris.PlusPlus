#include <Scenes/GameOverScene.h>
#include <Scenes/GameScene.h>
#include <Scenes/MainMenuScene.h>
#include <Renderer/TetrisVisuals.h>

#include <Engine/Game.h>
#include <Engine/Screen.h>

#include <Graphics/SpriteBatch.h>
#include <Graphics/Font.h>

#include <Content/Content.h>
#include <Input/Input.h>


struct GameOverScene::Impl
{
	explicit GameOverScene::Impl(int finalScore)
		: FinalScore(finalScore)
	{
	}

	const int FinalScore;
	std::unique_ptr<SpriteBatch> SpriteBatch;
	std::unique_ptr<Font> Font;
	std::unique_ptr<::Font> FontSmall;
};

GameOverScene::GameOverScene(int finalScore) : 
	FadeableScene(0.25f, Color::White), _pImpl(new GameOverScene::Impl(finalScore))
{
};

GameOverScene::~GameOverScene()
{
}

void GameOverScene::OnEntering()
{
	auto& graphicsContext = this->GetGame().GetGraphicsContext();
	_pImpl->SpriteBatch.reset(new SpriteBatch(graphicsContext));
	_pImpl->Font = Content::LoadFont(graphicsContext, "Fonts/Wonder.ttf", 32);
	_pImpl->FontSmall = Content::LoadFont(graphicsContext, "Fonts/Wonder.ttf", 24);
}

void GameOverScene::Update()
{
	if (Input::IsNewKeyPress(KeyCode::Enter))
	{
		this->ChangeScene(std::unique_ptr<Scene>(new GameScene()));
	}
	else if (Input::IsNewKeyPress(KeyCode::Escape))
	{
		this->ChangeScene(std::unique_ptr<Scene>(new MainMenuScene()));
	}
}

void GameOverScene::Render()
{
	_pImpl->SpriteBatch->Begin();
	_pImpl->SpriteBatch->DrawText(*_pImpl->Font, "GAME OVER", Vector2f(Screen::GetWidth() / 2.0f, 330), GetBlockColor(BlockType::Empty), TextCorner::Center);
	_pImpl->SpriteBatch->DrawText(*_pImpl->Font, "SCORE", Vector2f(Screen::GetWidth() / 2.0f, 420), GetBlockColor(BlockType::Empty), TextCorner::Center);
	_pImpl->SpriteBatch->DrawText(*_pImpl->Font, std::to_string(_pImpl->FinalScore), Vector2f(Screen::GetWidth() / 2.0f, 456), Color::MultiplyRGB(GetBlockColor(BlockType::Empty), 0.8f), TextCorner::Center);

	_pImpl->SpriteBatch->DrawText(*_pImpl->FontSmall, "PRESS ENTER TO REPLAY", Vector2f(Screen::GetWidth() / 2.0f, 560), Color::MultiplyRGB(GetBlockColor(BlockType::Empty), 1.1f), TextCorner::Center);
	_pImpl->SpriteBatch->End();
}