#include <Board.h>

#include <Diagnostics/Ensure.h>
#include <Math/Rectangle.h>

BlockType Board::At(int x, int y) const
{
	Ensure::WithinRange(x, 0, this->Width);
	Ensure::WithinRange(y, 0, this->Height);

	auto blockType = _blocks[x + y * this->Width];
	if (blockType != BlockType::Empty)
	{
		return blockType;
	}

	Vector2i blockSpacePosition = Vector2i(x, y) - _currentBlock.GetTopLeftPosition();
	if (_currentBlock.GetBoundingArea().Contains(blockSpacePosition))
	{
		return _currentBlock.GetData().At(blockSpacePosition.X, blockSpacePosition.Y) ? _currentBlock.Type : BlockType::Empty;
	}

	return BlockType::Empty;
}