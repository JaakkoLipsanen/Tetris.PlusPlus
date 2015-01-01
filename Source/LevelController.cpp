#include <LevelController.h>
#include <Level.h>
#include <Input/Input.h>

void LevelController::Control()
{
	if (Input::IsNewKeyPress(KeyCode::Up))
	{
		_level.GetBoard().RotateBlock();
	}

	_level.SetBlockFallingSpeed(Input::IsKeyPressed(KeyCode::Down) ? BlockFallingSpeed::Fast : BlockFallingSpeed::Normal);
	if (Input::IsKeyPressed(KeyCode::Left))
	{
		_level.MoveBlock(HorizontalDirection::Left);
	}
	else if (Input::IsKeyPressed(KeyCode::Right))
	{
		_level.MoveBlock(HorizontalDirection::Right);
	}

	if (Input::IsNewKeyPress(KeyCode::Space))
	{
		_level.GetBoard().InstantDropBlock();
	}

	if (Input::IsNewKeyPress(KeyCode::LeftShift))
	{
		_level.GetBoard().ToggleBlockHolding();
	}
}