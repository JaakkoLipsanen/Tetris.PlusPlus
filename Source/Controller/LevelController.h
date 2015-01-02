#pragma once

class Level;
class LevelController
{
public:
	explicit LevelController(Level& level) :
		_level(level)
	{
	}

	void Control();

private:
	Level& _level;
};