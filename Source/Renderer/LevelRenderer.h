#pragma once
#include <memory>

class GraphicsContext;
class Level;
class LevelRenderer
{
public:
	explicit LevelRenderer(const Level& level);
	~LevelRenderer();

	void LoadContent(GraphicsContext& graphicsContext);
	void Update();
	void Render();

private:
	struct Impl;
	std::unique_ptr<Impl> _pImpl;
};