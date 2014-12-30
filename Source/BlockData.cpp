#include <BlockData.h>
#include <Block.h>

#include <Diagnostics/Ensure.h>

bool BlockData::At(int x, int y) const
{
	Ensure::WithinRange(x, 0, this->Width);
	Ensure::WithinRange(y, 0, this->Height);

	return _data[x + y * this->Width];
}

void BlockData::Rotate(HorizontalDirection direction)
{
	Ensure::True(direction == HorizontalDirection::Right); // :P direction == Left not supported yet!

	auto copiedData = _data;
	for (int y = 0; y < this->Height; y++)
	{
		for (int x = 0; x < this->Width; x++)
		{
			int newY = x;
			int newX = this->Width - y;
			_data[newX + newY * this->Width] = copiedData[x + y * this->Width];
		}
	}
}

BlockData BlockData::FromBlockType(BlockType type)
{
	// horizontal line on the second line
	static const BlockData I = BlockData({ 4, 4, { { false, false, false, false }, { true, true, true, true }, { false, false, false, false }, { false, false, false, false } } });

	// 2x2 square on top-middle
	static const BlockData O = BlockData({ 2, 2, { { true, true }, { true, true }, } });

	// 3x3 upwards pointing 'T'
	static const BlockData T = BlockData({ 3, 3, { { false, true, false }, { true, true, true }, { false, false, false } } });

	// 3x3 'S' on top two lines
	static const BlockData S = BlockData({ 3, 3, { { false, true, true }, { true, true, false }, { false, false, false } } });

	// 3x3 'Z' on top two lines
	static const BlockData Z = BlockData({ 3, 3, { { true, true, false }, { false, true, true }, { false, false, false } } });

	// 3x3 'J' on top two lines
	static const BlockData J = BlockData({ 3, 3, { { true, false, false }, { true, true, true }, { false, false, false } } });

	// 3x3 'L' on top two lines
	static const BlockData L = BlockData({ 3, 3, { { false, false, true }, { true, true, true }, { false, false, false } } });

	switch (type)
	{
	case BlockType::Empty:
		throw std::invalid_argument("'Empty' doesn't have data");

	case BlockType::I: return I;
	case BlockType::O: return O;
	case BlockType::T: return T;
	case BlockType::S: return S;
	case BlockType::Z: return Z;
	case BlockType::J: return J;
	case BlockType::L: return L;

	default:
		throw std::invalid_argument("Unknown block type");
	}
}