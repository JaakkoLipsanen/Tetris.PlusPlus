#pragma once
#include <vector>

struct Rectangle;
enum class BlockType;
enum class HorizontalDirection;

class BlockData
{
public:
	const int Width;
	const int Height;

	explicit BlockData(int width, int height, bool** data)
		: Width(width), Height(height), _data(data, data + width * height)
	{
	}

	bool At(int x, int y) const;
	void Rotate(HorizontalDirection direction);

	static BlockData FromBlockType(BlockType type);

private:
	std::vector<bool> _data;
};
