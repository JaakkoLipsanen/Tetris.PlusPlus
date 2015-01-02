#include <Engine/Game.h>
#include <Scenes/MainMenuScene.h>
#include <Core/WindowDescription.h>

#include <Core/Color.h>
#include <Core/IGameWindow.h>
#include <Graphics/GraphicsContext.h>

#include <Graphics/Post Processing/PostProcessRenderer.h>
#include <Graphics/Post Processing/Vignette.h>
#include <Graphics/Post Processing/ColorAdjust.h>

class TetrisGame : public Game
{
protected:
	std::unique_ptr<Scene> CreateDefaultScene() override
	{
		return std::unique_ptr<Scene>(new MainMenuScene());
	}

	void SetupWindow(WindowDescription& description) override
	{
		description.Resolution = Size(720, 720);
		description.HasBorders = true;
	}

	void Initialize() override
	{
		this->GetWindow().SetPosition(Vector2i(750, 350));

		auto& graphicsContext = this->GetGraphicsContext();
		this->PostProcessRenderer.reset(new ::PostProcessRenderer(graphicsContext));

		this->PostProcessRenderer->AddPostProcess(std::make_shared<ColorAdjustPostProcess>(graphicsContext));
		this->PostProcessRenderer->AddPostProcess(std::make_shared<VignettePostProcess>(graphicsContext));

		this->PostProcessRenderer->Get<ColorAdjustPostProcess>()->SetSaturationMultiplier(0.4f);
	}

	void PreRender() override
	{
		this->PostProcessRenderer->BeginRender();
		this->GetGraphicsContext().Clear(Color::White);
	}

	void PostRender() override
	{
		this->PostProcessRenderer->Render(nullptr);
	}

private:
	std::unique_ptr<PostProcessRenderer> PostProcessRenderer;
};

int main()
{
	TetrisGame game;
	game.Run();

	return 1;
}