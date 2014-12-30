#pragma once
#include <BlockData.h>
#include <Math/Direction.h>

enum class BlockType
{
	Empty,

	I, // ----

	O, // --
	// --

	T, // ---
	//  -

	S, //  --
	// --

	Z, // --
	//  --

	J, // -
	// ---

	L, //   -
	// ---
};

class Block
{
public:
	const BlockType Type;

	explicit Block(BlockType type);
	void MoveDown();

	// returns true if move was possible, false otherwise
	bool MoveHorizontally(HorizontalDirection direction);
	void Rotate();

	inline const BlockData& GetData() const {
		return _data;
	}

	inline Vector2i GetTopLeftPosition() const {
		return _topLeftPosition;
	}


	Rectangle GetBoundingArea() const;

private:
	Vector2i _topLeftPosition;
	BlockData _data;
};
