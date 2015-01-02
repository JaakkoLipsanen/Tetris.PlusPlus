#include <Scenes/MainMenuScene.h>
#include <Scenes/GameScene.h>
#include <Renderer/TetrisVisuals.h>

#include <Graphics/SpriteBatch.h>
#include <Graphics/Font.h>
#include <Content/Content.h>

#include <Engine/Screen.h>
#include <Engine/Game.h>
#include <Input/Input.h>

struct MainMenuScene::Impl
{
	std::unique_ptr<SpriteBatch> SpriteBatch;
	std::unique_ptr<Font> Font;
	std::unique_ptr<::Font> FontSmall;
};

MainMenuScene::MainMenuScene() : 
	FadeableScene(0.25f, Color::White),  _pImpl(new MainMenuScene::Impl())
{
}
MainMenuScene::~MainMenuScene() = default;

void MainMenuScene::OnEntering()
{
	auto& graphicsContext = this->GetGame().GetGraphicsContext();
	_pImpl->SpriteBatch.reset(new SpriteBatch(graphicsContext));
	_pImpl->Font = Content::LoadFont(graphicsContext, "Fonts/Wonder.ttf", 32);
	_pImpl->FontSmall = Content::LoadFont(graphicsContext, "Fonts/Wonder.ttf", 22);
}

void MainMenuScene::Update()
{
	if (Input::IsNewKeyPress(KeyCode::Enter))
	{
		this->ChangeScene(std::unique_ptr<Scene>(new GameScene()));
	}
	else if (Input::IsNewKeyPress(KeyCode::Escape))
	{
		this->GetGame().Exit();
	}
}

void MainMenuScene::Render()
{
	_pImpl->SpriteBatch->Begin();
	_pImpl->SpriteBatch->DrawText(*_pImpl->Font, "Press Enter to Play", Vector2f(Screen::GetWidth() / 2.0f, 330), GetBlockColor(BlockType::Empty), TextCorner::Center);
	_pImpl->SpriteBatch->DrawText(*_pImpl->FontSmall, "Press Escape to Exit", Vector2f(Screen::GetWidth() / 2.0f, 360), Color::MultiplyRGB(GetBlockColor(BlockType::Empty), 1.1f), TextCorner::Center);

	Color controlsColor = Color::MultiplyRGB(GetBlockColor(BlockType::Empty), 1.1f);
	_pImpl->SpriteBatch->DrawText(*_pImpl->FontSmall, "Move with Left and Right", Vector2f(Screen::GetWidth() / 2.0f, 450), controlsColor, TextCorner::Center);
	_pImpl->SpriteBatch->DrawText(*_pImpl->FontSmall, "Rotate with Up", Vector2f(Screen::GetWidth() / 2.0f, 475), controlsColor, TextCorner::Center);
	_pImpl->SpriteBatch->DrawText(*_pImpl->FontSmall, "Soft Drop with Down", Vector2f(Screen::GetWidth() / 2.0f, 500), controlsColor, TextCorner::Center);
	_pImpl->SpriteBatch->DrawText(*_pImpl->FontSmall, "Hard Drop with Space", Vector2f(Screen::GetWidth() / 2.0f, 525), controlsColor, TextCorner::Center);
	_pImpl->SpriteBatch->End();
}