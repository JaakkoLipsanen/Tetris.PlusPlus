#include <Renderer/LevelRenderer.h>
#include <Model/Level.h>

#include <Graphics/SpriteBatch.h>
#include <Graphics/TextureHelper.h>
#include <Graphics/Font.h>
#include <Graphics/Post Processing/PostProcessRenderer.h>
#include <Graphics/Post Processing/Vignette.h>
#include <Graphics/Post Processing/ColorAdjust.h>

#include <Content/Content.h>
#include <Engine/Screen.h>

static const int BlockSize = 32;
Color GetBlockColor(BlockType type)
{
	switch (type)
	{
	case BlockType::Empty: return Color(192, 192, 192);
	case BlockType::I: return Color::Cyan;
	case BlockType::J: return Color::Blue;
	case BlockType::L: return Color::Orange;
	case BlockType::O: return Color::Yellow;
	case BlockType::S: return Color::Lime;
	case BlockType::T: return Color::DarkMagenta;
	case BlockType::Z: return Color::Red;

	default:
		throw std::invalid_argument("Invalid type");
	}
}

struct LevelRenderer::Impl
{
	explicit LevelRenderer::Impl(const Level& level) :
		Level(level)
	{
	}

	void DrawBoard()
	{
		auto& board = this->Level.GetBoard();

		Vector2f startLocation = (static_cast<Vector2f>(Screen::GetSize().ToVector2i()) - Vector2f(board.Width, board.VisibleHeight) * BlockSize) / 2.0f;
		for (int y = 0; y < board.VisibleHeight; y++)
		{
			for (int x = 0; x < board.Width; x++)
			{
				this->DrawSingleBlock(startLocation + Vector2f(x, y) * BlockSize, board.AtVisually(x, y));
			}
		}
	}


	// ghost block == the placement of the block where the current block will be if it falls straight down
	void DrawGhostBlock()
	{
		static const float GhostBlockAlpha = 0.35f;
		auto& board = this->Level.GetBoard();

		Block ghostBlock = board.GetGhostBlock();

		Vector2f startLocation = (static_cast<Vector2f>(Screen::GetSize().ToVector2i()) - Vector2f(board.Width, board.VisibleHeight) * BlockSize) / 2.0f;
		Vector2f blockAdjustment = Vector2f(ghostBlock.GetBottomLeftPosition().X, Board::VisibleHeight - ghostBlock.GetBoundingArea().MaxY()) * BlockSize;
		this->DrawBlock(startLocation + blockAdjustment, ghostBlock.GetData(), ghostBlock.Type, GhostBlockAlpha);
	}

	void DrawBlock(Vector2f position, const BlockData& data, BlockType type, float alpha = 1)
	{
		for (int y = 0; y < data.Height; y++)
		{
			for (int x = 0; x < data.Width; x++)
			{
				int testY = data.Height - 1 - y;
				if (data.At(x, testY))
				{
					Vector2f location = Vector2f(x, y);
					this->DrawSingleBlock(position + location * BlockSize, type, alpha);
				}
			}
		}
	}
	

	void DrawSingleBlock(Vector2f position, BlockType type, float alpha = 1)
	{
		this->SpriteBatch->Draw(*this->BlankPixel, position, GetBlockColor(type) * alpha, 0, Vector2f::One * BlockSize);
	}

	void DrawPreviewBlock(Vector2f position, BlockType type, const std::string& title)
	{
		static const float Size = 4.5f;
		this->SpriteBatch->Draw(*this->BlankPixel, position, GetBlockColor(BlockType::Empty), 0, Vector2f::One * BlockSize * Size);

		float centerX = position.X + (Size / 2.0f) * BlockSize;
		this->SpriteBatch->DrawText(*this->Font, title, Vector2f(centerX, position.Y - this->Font->MeasureText(title).Height / 2.0f - 8), GetBlockColor(BlockType::Empty), TextCorner::Center);

		if (type != BlockType::Empty)
		{
			auto data = BlockData::FromBlockType(type).GetTrimmed();
			this->DrawBlock(position + Vector2f((Size - data.Width) / 2.0f, (Size - data.Height) / 2.0f) * BlockSize, data, type, 1);
		}
	}

	GraphicsContext* GraphicsContext;
	std::unique_ptr<Texture2D> BlankPixel;
	std::unique_ptr<SpriteBatch> SpriteBatch;
	std::unique_ptr<Font> Font;
	std::unique_ptr<::Font> FontSmall;
	std::unique_ptr<PostProcessRenderer> PostProcessRenderer;
	const Level& Level;
};

LevelRenderer::LevelRenderer(const Level& level)
	: _pImpl(new LevelRenderer::Impl(level))
{
}
LevelRenderer::~LevelRenderer() = default;

void LevelRenderer::LoadContent(GraphicsContext& graphicsContext)
{
	_pImpl->GraphicsContext = &graphicsContext;
	_pImpl->SpriteBatch.reset(new SpriteBatch(graphicsContext));
	_pImpl->BlankPixel = TextureHelper::CreateBlankTexture(graphicsContext);
	_pImpl->Font = Content::LoadFont(graphicsContext, "Fonts/Wonder.ttf", 32);
	_pImpl->FontSmall = Content::LoadFont(graphicsContext, "Fonts/Wonder.ttf", 24);
	_pImpl->PostProcessRenderer.reset(new PostProcessRenderer(graphicsContext));

	_pImpl->PostProcessRenderer->AddPostProcess(std::make_shared<ColorAdjustPostProcess>(graphicsContext));
	_pImpl->PostProcessRenderer->AddPostProcess(std::make_shared<VignettePostProcess>(graphicsContext));
}

void LevelRenderer::Update()
{
	_pImpl->PostProcessRenderer->Get<ColorAdjustPostProcess>()->SetSaturationMultiplier(
		0.4f); //FlaiMath::PingPong(Time::GetTotalTime(), 1.0f));
}

void LevelRenderer::Render()
{
	_pImpl->PostProcessRenderer->BeginRender();
	_pImpl->GraphicsContext->Clear(Color::White);

	_pImpl->SpriteBatch->Begin();
	_pImpl->DrawBoard();
	_pImpl->DrawGhostBlock();
	_pImpl->DrawPreviewBlock(Vector2f(26, 48), _pImpl->Level.GetBoard().GetNextBlock(), "Next");
	_pImpl->DrawPreviewBlock(Vector2f(Screen::GetWidth() - 26 * 2 - 26 * 4.5f, 48), _pImpl->Level.GetBoard().GetHoldedBlock(), "Hold");
	_pImpl->SpriteBatch->DrawText(*_pImpl->FontSmall, "Score", Vector2f(100, 214), GetBlockColor(BlockType::Empty), TextCorner::Center);
	_pImpl->SpriteBatch->DrawText(*_pImpl->FontSmall, std::to_string(_pImpl->Level.GetCurrentScore()), Vector2f(100, 240), GetBlockColor(BlockType::Empty), TextCorner::Center);

	_pImpl->SpriteBatch->End();

	_pImpl->PostProcessRenderer->Render(nullptr);
}