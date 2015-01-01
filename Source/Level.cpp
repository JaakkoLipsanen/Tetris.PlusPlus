#include <Level.h>

#include <Engine/Time.h>
#include <Core/Array.h>
#include <Diagnostics/Ensure.h>

static const float BlockFallingRate = 0.4f;
static const float BlockHorizontalMoveRate = 0.075f;

static const int LinesClearedScoring[] = {
	0,
	100, // 1 line cleared == 100p 
	300, // 2 == 300p
	500, // and etc 
	800
};

struct Level::Impl
{
	Level::Impl()
		: Board(std::unique_ptr<IBlockGenerator>(new BlockGenerator))
	{
		this->Board.LinesCleared += [this](int linesCleared)
		{
			Ensure::WithinRange(linesCleared, 0, static_cast<int>(Array::Length(LinesClearedScoring)));
			this->CurrentScore += LinesClearedScoring[linesCleared];
		};
	}

	void UpdateTimeSinceFall()
	{
		this->TimeSinceLastFall -= Time::GetDeltaTime() * (this->FallingSpeed == BlockFallingSpeed::Normal ? 1 : 8);
		if (this->TimeSinceLastFall <= 0)
		{
			bool didMove = this->Board.TickVertically();
			this->TimeSinceLastFall += BlockFallingRate;

			if (didMove && this->FallingSpeed == BlockFallingSpeed::Fast)
			{
				this->CurrentScore += 1; // every move that is made during "soft drop" grants 1 point
			}
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
	int CurrentScore = 0;
};

Level::Level() :
	_pImpl(new Level::Impl())
{
}
Level::~Level() = default;

Board& Level::GetBoard() { return _pImpl->Board; }
const Board& Level::GetBoard() const { return _pImpl->Board; }

void Level::InstantDropBlock()
{
	auto ghost = _pImpl->Board.GetGhostBlock();
	int distanceToGround = _pImpl->Board.GetCurrentBlock().GetBottomLeftPosition().Y - ghost.GetBottomLeftPosition().Y;

	_pImpl->CurrentScore += 2 * distanceToGround; // hard drop grants 2 points for every cell the block drops
	this->GetBoard().InstantDropBlock();
}

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

int Level::GetCurrentScore() const
{
	return _pImpl->CurrentScore;
}