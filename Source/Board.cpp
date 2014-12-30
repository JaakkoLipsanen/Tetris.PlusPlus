#include <Board.h>
#include <Diagnostics/Ensure.h>

BlockType Board::At(int x, int y) const
{
	Ensure::WithinRange(x, 0, this->Width);
	Ensure::WithinRange(y, 0, this->Height);

	auto blockType = _blocks[x + y * this->Width];
	if (blockType != BlockType::Empty)
	{
		return blockType;
	}

	return _currentBlock.
}