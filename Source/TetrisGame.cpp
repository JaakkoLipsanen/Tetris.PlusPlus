#include <Engine/Game.h>
#include <GameScene.h>
#include <Core/WindowDescription.h>

#include <Core/Color.h>
#include <Graphics/GraphicsContext.h>
#include <Core/IGameWindow.h>

class TetrisGame : public Game
{
protected:
	std::unique_ptr<Scene> CreateDefaultScene() override
	{
		return std::unique_ptr<Scene>(new GameScene());
	}

	void SetupWindow(WindowDescription& description) override
	{
		description.Resolution = Size(720, 720);
		description.HasBorders = false;
	}

	void Initialize() override
	{
		this->GetWindow().SetPosition(Vector2i(750, 350));
	}

	void PreRender() override
	{
		this->GetGraphicsContext().Clear(Color::White);
	}
};

int main()
{
	TetrisGame game;
	game.Run();

	return 1;
}