#pragma once
#include <Model/Board.h>

enum class BlockFallingSpeed
{
	Normal,
	Fast,
};

class Level
{
public:
	Event<void(int)> GameOver; // int parameter == final score of the game

	Level();
	~Level();
	void Update();

	int GetCurrentScore() const;
	Board& GetBoard();
	const Board& GetBoard() const;

	void InstantDropBlock();
	void MoveBlock(HorizontalDirection direction);
	void SetBlockFallingSpeed(BlockFallingSpeed fallingSpeed);

private:
	struct Impl;
	std::unique_ptr<Impl> _pImpl;
};