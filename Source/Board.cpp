#include <Board.h>

#include <Diagnostics/Ensure.h>
#include <Math/Rectangle.h>

struct Board::Impl
{
	explicit Board::Impl(std::unique_ptr<IBlockGenerator> generator) :
		Generator(std::move(generator)), HoldedBlock(BlockType::Empty)
	{
		this->SpawnNextBlock();
		std::fill(std::begin(this->Blocks), std::end(this->Blocks), BlockType::Empty);
	}

	void SpawnNextBlock()
	{
		this->SpawnNextBlock(this->Generator->Next());
	}

	void SpawnNextBlock(BlockType type)
 	{
		this->CurrentBlock.reset(new Block(type));
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
		this->SpawnNextBlock();
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
			}
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

	BlockType Blocks[Board::Width * Board::Height];
	std::unique_ptr<IBlockGenerator> Generator;
	std::unique_ptr<Block> CurrentBlock; // cannot be plain "Block CurrentBlock;" since Block is uncopyable...
	BlockType HoldedBlock;
};

Board::Board(std::unique_ptr<IBlockGenerator> generator) :
	_pImpl(new Board::Impl(std::move(generator)))
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
	auto ghost = this->GetGhostBlock();
	_pImpl->FreezeBlock(ghost);
	_pImpl->SpawnNextBlock();
}

void Board::TickVertically()
{
	Block copy = *_pImpl->CurrentBlock;
	copy.MoveDown();
	if (_pImpl->CollidesWithBlocks(copy)) // the current block would be colliding with other blocks after moving.
	{
		_pImpl->FreezeCurrentBlock();

		return;
	}

	_pImpl->CurrentBlock->MoveDown();
}

void Board::ToggleBlockHolding()
{
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