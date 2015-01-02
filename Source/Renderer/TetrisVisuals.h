#pragma once
#include <Model/Block.h>
#include <Core/Color.h>

static const int BlockSize = 32;
inline Color GetBlockColor(BlockType type)
{
	switch (type)
	{
	case BlockType::Empty: return Color(192, 192, 192);
	case BlockType::I: return Color::Cyan;
	case BlockType::J: return Color::Blue;
	case BlockType::L: return Color::Orange;
	case BlockType::O: return Color::Yellow;
	case BlockType::S: return Color::Lime;
	case BlockType::T: return Color::DarkMagenta;
	case BlockType::Z: return Color::Red;

	default:
		throw std::invalid_argument("Invalid type");
	}
}