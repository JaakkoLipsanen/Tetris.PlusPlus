#pragma once
#include <Model/Block.h>

class IBlockGenerator
{
public:
	virtual ~IBlockGenerator() = default;
	virtual BlockType Next() = 0;
	virtual BlockType PeekNext() const = 0;
};

class BlockGenerator : public IBlockGenerator
{
public:
	BlockType Next() override;
	BlockType PeekNext() const override;

private:
	void GeneratePermutation();
	std::vector<BlockType> _blockBag;
};