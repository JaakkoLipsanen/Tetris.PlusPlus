#pragma once
#include <Block.h>
#include <BlockGenerator.h>
#include <Core/Event.h>

#include <memory>

class Board
{
public:
	static const int Width = 10;
	static const int Height = 24;
	static const int VisibleHeight = 20;

	Event<void(int)> LinesCleared; // the int-parameter tells how many lines were cleared
	Event<void()> GameOver;

	explicit Board(std::unique_ptr<IBlockGenerator> generator);
	~Board();

	BlockType At(int x, int y) const; // y == 0 is at bottom
	BlockType AtVisually(int x, int y) const; // in this scenario, y == 0 is at top (of VisibleHeight)

	void MoveBlock(HorizontalDirection direction);
	void InstantDropBlock();
	bool TickVertically(); // returns true if current block moved one block down succesfully; false otherwise
	void ToggleBlockHolding();
	void RotateBlock();

	BlockType GetNextBlock() const;
	BlockType GetHoldedBlock() const;

	const Block& GetCurrentBlock() const;
	Block GetGhostBlock() const;

private:
	struct Impl;
	std::unique_ptr<Impl> _pImpl;
};