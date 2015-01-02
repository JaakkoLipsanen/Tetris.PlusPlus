#include <Model/BlockGenerator.h>
#include <Core/Array.h>
#include <Engine/Time.h>

#include <random>
#include <Diagnostics/Ensure.h>

BlockType BlockGenerator::Next()
{
	if (_blockBag.empty()) {
		this->GeneratePermutation();
	}

	auto next = this->PeekNext();
	_blockBag.pop_back();

	if (_blockBag.empty()) {
		this->GeneratePermutation();
	}

	return next;
}

BlockType BlockGenerator::PeekNext() const
{
	Ensure::False(_blockBag.empty(), "The bag is empty!");
	return _blockBag.back();
}

void BlockGenerator::GeneratePermutation()
{
	static const BlockType Values[] = { BlockType::I, BlockType::O, BlockType::T, BlockType::S, BlockType::Z, BlockType::J, BlockType::L };
	_blockBag.assign(Values, Values + Array::Length(Values));

	// shuffle the values \o/
	std::shuffle(_blockBag.begin(), _blockBag.end(), std::default_random_engine(Time::GetSystemTime()));
}