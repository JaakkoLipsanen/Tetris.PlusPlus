#pragma once
#include <Block.h>
#include <BlockGenerator.h>
#include <memory>

class Board
{
	explicit Board( std::unique_ptr<IBlockGenerator> generator)
		: _generator(std::move(generator)),  _currentBlock(Block(_generator->Next()))
	{
	}

public:
	static const int Width = 10;
	static const int Height = 22;
	static const int VisibleHeight = 20;

	BlockType At(int x, int y) const;

private:
	BlockType _blocks[Board::Width * Board::Height];
	std::unique_ptr<IBlockGenerator> _generator;
	Block _currentBlock;
};