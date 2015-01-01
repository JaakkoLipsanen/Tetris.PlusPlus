#pragma once
#include <Board.h>

enum class BlockFallingSpeed
{
	Normal,
	Fast,
};

class Level
{
public:
	Level();
	~Level();
	void Update();

	Board& GetBoard();
	const Board& GetBoard() const;

	void MoveBlock(HorizontalDirection direction);
	void SetBlockFallingSpeed(BlockFallingSpeed fallingSpeed);

private:
	struct Impl;
	std::unique_ptr<Impl> _pImpl;
};