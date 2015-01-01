#include <Block.h>
#include <Board.h>
#include <BlockData.h>

#include <Diagnostics/Ensure.h>
#include <Math/Rectangle.h>

/* Block */
static Vector2i GetSpawnLocation(BlockType type)
{
	switch (type)
	{
	case BlockType::Empty:
		throw std::invalid_argument("'Empty' cannot be spawned");

	case BlockType::I:
		return Vector2i(Board::Width / 2 - 2, Board::VisibleHeight); // centered horizontally (4x4)
	case BlockType::O:
		return Vector2i(Board::Width / 2 - 1, Board::VisibleHeight + 1); // centered horizontally (2x2)

	default:
		return Vector2i(Board::Width / 2 - 2, Board::VisibleHeight + 1); // all the other blocks (3x3). Spawns so that horizontally 2 blocks are before the mid-point and 1 block after
	}
}

Block::Block(BlockType type)
	: Type(type), _bottomLeftPosition(GetSpawnLocation(type)), _data(BlockData::FromBlockType(type))
{
}

void Block::MoveDown()
{
//	Ensure::True(_bottomLeftPosition.Y > 0);
	_bottomLeftPosition.Y--;
}

bool Block::MoveHorizontally(HorizontalDirection direction)
{
	// todo: check if move is possible
	_bottomLeftPosition.X += static_cast<int>(direction);
	return true;
}

void Block::Rotate()
{
	_data.Rotate(HorizontalDirection::Right);
}

Rectangle Block::GetBoundingArea() const 
{
	return Rectangle(_bottomLeftPosition.X, _bottomLeftPosition.Y, _data.Width, _data.Height);
}

Rectangle Block::GetBlockArea() const
{
	Rectangle area = this->GetBoundingArea();
	area.Width--;
	area.Height--;

	return area;
}