#include <Board.h>

#include <Diagnostics/Ensure.h>
#include <Math/Rectangle.h>

struct Board::Impl
{
	explicit Board::Impl(std::unique_ptr<IBlockGenerator> generator, Event<void(int)>& lineClearedEvent, Event<void()>& gameOver) :
		Generator(std::move(generator)), HoldedBlock(BlockType::Empty), LinesCleared(lineClearedEvent), GameOver(gameOver)
	{
		std::fill(std::begin(this->Blocks), std::end(this->Blocks), BlockType::Empty);
		this->SpawnNextBlock();
	}

	void SpawnNextBlock()
	{
		this->SpawnNextBlock(this->Generator->Next());
	}

	void SpawnNextBlock(BlockType type)
 	{
		if (this->IsGameOver)
		{
			return;
		}

		this->CurrentBlock.reset(new Block(type));
		if (this->CollidesWithBlocks(*this->CurrentBlock))
		{
			this->EndGame();
		}
	}

	BlockType AtInternal(int x, int y, bool allowFallingBlock)
	{
		Ensure::WithinRange(x, 0, Board::Width);
		Ensure::WithinRange(y, 0, Board::Height);

		auto blockType = this->Blocks[x + y * Board::Width];
		if (blockType != BlockType::Empty)
		{
			return blockType;
		}

		if (allowFallingBlock && this->CurrentBlock->GetBlockArea().Contains(Vector2i(x, y)))
		{
			Vector2i blockSpacePosition = Vector2i(x, y) - this->CurrentBlock->GetBoundingArea().BottomLeft();
			return this->CurrentBlock->GetData().At(blockSpacePosition.X, blockSpacePosition.Y) ? this->CurrentBlock->Type : BlockType::Empty;
		}

		return BlockType::Empty;
	}

	bool CollidesWithBlocks(Block& block)
	{
		auto& data = block.GetData();
		for (int y = 0; y < data.Height; y++)
		{
			for (int x = 0; x < data.Width; x++)
			{
				if (data.At(x, y) == true)
				{
					Vector2i realIndex = Vector2i(x, y) + block.GetBottomLeftPosition();
					if (realIndex.Y < 0 || realIndex.Y >= Board::Height || realIndex.X < 0 || realIndex.X >= Board::Width)
					{
						return true;
					}

					if (this->AtInternal(realIndex.X, realIndex.Y, false) != BlockType::Empty)
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	void FreezeCurrentBlock()
	{
		this->FreezeBlock(*this->CurrentBlock);  
		if (!this->IsGameOver)
		{
			this->SpawnNextBlock();
		}
	}

	void FreezeBlock(Block& block)
	{
		auto& data = block.GetData();
		for (int y = 0; y < data.Height; y++)
		{
			for (int x = 0; x < data.Width; x++)
			{
				if (data.At(x, y))
				{
					Vector2i realIndex = Vector2i(x, y) + block.GetBottomLeftPosition();
					if (realIndex.Y >= Board::VisibleHeight)
					{
						this->EndGame();
						return;
					}

					if (realIndex.Y < Board::Height && realIndex.Y >= 0)
					{
						this->Blocks[realIndex.X + realIndex.Y * Board::Width] = block.Type;
					}
				}
			}
		}

		this->CheckForLineCompletion();
	}

	void CheckForLineCompletion()
	{
		int linesRemoved = 0;
		for (int y = 0; y < Board::Height; y++)
		{
			bool isFullLine = true;
			for (int x = 0; x < Board::Width; x++)
			{
				if (this->AtInternal(x, y, false) == BlockType::Empty)
				{
					isFullLine = false;
					break;
				}
			}

			if (isFullLine)
			{
				this->DeleteLine(y);
				y--; // since all the above lines were moved down, we need to compensate it in the 'y'
				linesRemoved++;
			}
		}

		if (linesRemoved > 0)
		{
			this->LinesCleared.Invoke(linesRemoved);
		}
	}

	void DeleteLine(int y)
	{
		for (int j = y + 1; j < Board::Height; j++)
		{
			int targetLine = j - 1; // to which line the blocks are moved
			for (int x = 0; x < Board::Width; x++)
			{
				this->Blocks[x + targetLine * Board::Width] = this->Blocks[x + j * Board::Width];
			}
		}
	}

	void EndGame()
	{
		this->IsGameOver = true;
		this->GameOver.Invoke();
	}

	BlockType Blocks[Board::Width * Board::Height];
	std::unique_ptr<IBlockGenerator> Generator;
	std::unique_ptr<Block> CurrentBlock; // cannot be plain "Block CurrentBlock;" since Block is uncopyable...
	BlockType HoldedBlock;

	Event<void(int)>& LinesCleared;
	Event<void()>& GameOver;
	bool IsGameOver = false;
};

Board::Board(std::unique_ptr<IBlockGenerator> generator) :
	LinesCleared(), _pImpl(new Board::Impl(std::move(generator), this->LinesCleared, this->GameOver))
{
}
Board::~Board() = default;

BlockType Board::At(int x, int y) const
{
	return _pImpl->AtInternal(x, y, true);
}

// in this case, y starts from top (y == 0 is really Board::VisualHeight - 1)
BlockType Board::AtVisually(int x, int y) const
{
	Ensure::WithinRange(x, 0, this->Width);
	Ensure::WithinRange(y, 0, this->VisibleHeight);

	return _pImpl->AtInternal(x, this->VisibleHeight - 1 - y, true);
}

void Board::MoveBlock(HorizontalDirection direction)
{
	if (_pImpl->IsGameOver)
	{
		return;
	}

	Block copy = *_pImpl->CurrentBlock;
	copy.MoveHorizontally(direction);
	if (_pImpl->CollidesWithBlocks(copy)) // the current block would be colliding with other blocks after moving.
	{
		return;
	}

	_pImpl->CurrentBlock->MoveHorizontally(direction);
}

void Board::InstantDropBlock()
{
	if (_pImpl->IsGameOver)
	{
		return;
	}

	auto ghost = this->GetGhostBlock();
	_pImpl->FreezeBlock(ghost);
	_pImpl->SpawnNextBlock();
}

bool Board::TickVertically()
{
	if (_pImpl->IsGameOver)
	{
		return false;
	}

	Block copy = *_pImpl->CurrentBlock;
	copy.MoveDown();
	if (_pImpl->CollidesWithBlocks(copy)) // the current block would be colliding with other blocks after moving.
	{
		_pImpl->FreezeCurrentBlock();
		return false; // block didnt move down
	}

	_pImpl->CurrentBlock->MoveDown();
	return true; // block moved one cell down
}

void Board::ToggleBlockHolding()
{
	if (_pImpl->IsGameOver)
	{
		return;
	}

	if (_pImpl->HoldedBlock == BlockType::Empty)
	{
		_pImpl->HoldedBlock = _pImpl->CurrentBlock->Type;
		_pImpl->SpawnNextBlock();
	}
	else
	{
		_pImpl->SpawnNextBlock(_pImpl->HoldedBlock);
		_pImpl->HoldedBlock = BlockType::Empty;
	}
}

void Board::RotateBlock()
{
	if (_pImpl->IsGameOver)
	{
		return;
	}

	Block copy = *_pImpl->CurrentBlock;
	copy.Rotate();
	if (_pImpl->CollidesWithBlocks(copy)) // the current block would be colliding with other blocks after moving.
	{
		return;
	}

	_pImpl->CurrentBlock->Rotate();
}

BlockType Board::GetHoldedBlock() const
{
	return _pImpl->HoldedBlock;
}

const Block& Board::GetCurrentBlock() const
{
	return *_pImpl->CurrentBlock;
}

BlockType Board::GetNextBlock() const
{
	return _pImpl->Generator->PeekNext();
}

Block Board::GetGhostBlock() const
{
	Block copy = *_pImpl->CurrentBlock;
	while (true)
	{
		Block previous = copy;
		copy.MoveDown();
		if (_pImpl->CollidesWithBlocks(copy))
		{
			return previous;
		}
	}
}