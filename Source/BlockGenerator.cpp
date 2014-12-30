#include <BlockGenerator.h>
#include <Diagnostics/Ensure.h>
#include <Core/Array.h>
#include <random>

BlockType BlockGenerator::Next()
{
	if (_blockBag.size() == 0)
	{
		this->GeneratePermutation();
	}

	auto next = this->PeekNext();
	_blockBag.pop_back();
	return next;
}

BlockType BlockGenerator::PeekNext() const
{
	Ensure::True(_blockBag.size() > 0);
	return _blockBag.back();
}

void BlockGenerator::GeneratePermutation()
{
	static const BlockType Values[] = { BlockType::I, BlockType::O, BlockType::T, BlockType::S, BlockType::Z, BlockType::J, BlockType::L };
	_blockBag.assign(Values, Values + Array::Length(Values));

	// shuffle the values \o/
	std::shuffle(_blockBag.begin(), _blockBag.end(), std::default_random_engine());
}