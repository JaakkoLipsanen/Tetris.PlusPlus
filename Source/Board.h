#pragma once
#include <Block.h>

class Board
{
	explicit Board()
		: _currentBlock(Block(BlockType::I))
	{
	}

public:
	static const int Width = 10;
	static const int Height = 22;
	static const int VisibleHeight = 20;

	BlockType At(int x, int y) const;

private:
	BlockType _blocks[Board::Width * Board::Height];
	Block _currentBlock;
};