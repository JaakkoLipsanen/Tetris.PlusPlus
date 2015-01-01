#include <Level.h>
#include <Engine/Time.h>

static const float BlockFallingRate = 0.4f;
static const float BlockHorizontalMoveRate = 0.075f;

struct Level::Impl
{
	Level::Impl()
		: Board(std::unique_ptr<IBlockGenerator>(new BlockGenerator))
	{
	}

	void UpdateTimeSinceFall()
	{
		this->TimeSinceLastFall -= Time::GetDeltaTime() * (this->FallingSpeed == BlockFallingSpeed::Normal ? 1 : 8);
		if (this->TimeSinceLastFall <= 0)
		{
			this->Board.TickVertically();
			this->TimeSinceLastFall += BlockFallingRate;
		}
	}

	void UpdateTimeSinceMove()
	{
		if (this->TimeSinceLastMove > 0)
		{
			this->TimeSinceLastMove -= Time::GetDeltaTime();
		}

		if (this->TimeSinceLastMove <= 0 && (this->MovingDirection == HorizontalDirection::Left || this->MovingDirection == HorizontalDirection::Right))
		{
			this->Board.MoveBlock(this->MovingDirection);
			this->TimeSinceLastMove += BlockHorizontalMoveRate;
		}

		this->MovingDirection = static_cast<HorizontalDirection>(0);
	}

	Board Board;
	BlockFallingSpeed FallingSpeed = BlockFallingSpeed::Normal;
	HorizontalDirection MovingDirection = static_cast<HorizontalDirection>(0);

	float TimeSinceLastFall = BlockFallingRate;
	float TimeSinceLastMove = BlockHorizontalMoveRate;
};

Level::Level() :
	_pImpl(new Level::Impl())
{
}
Level::~Level() = default;

Board& Level::GetBoard() { return _pImpl->Board; }
const Board& Level::GetBoard() const { return _pImpl->Board; }

void Level::MoveBlock(HorizontalDirection direction)
{
	_pImpl->MovingDirection = direction;
	_pImpl->TimeSinceLastFall = BlockFallingRate;
}

void Level::SetBlockFallingSpeed(BlockFallingSpeed fallingSpeed)
{
	_pImpl->FallingSpeed = fallingSpeed;
}

void Level::Update()
{
	_pImpl->UpdateTimeSinceMove();
	_pImpl->UpdateTimeSinceFall();
}