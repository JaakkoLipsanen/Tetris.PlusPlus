#include <Engine/Game.h>
#include <Core/WindowDescription.h>

#include <GameScene.h>
#include <Core/Color.h>
#include <Graphics/GraphicsContext.h>

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
	}

	void PostRender() override
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