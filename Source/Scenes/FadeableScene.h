#pragma once
#include <Engine/Scene.h>
#include <Core/Color.h>
#include <memory>

class FadeableScene : public Scene
{
public:
	virtual ~FadeableScene() override;
	void ChangeScene(std::unique_ptr<Scene> sceneToLoad);

	virtual void PostUpdate() override;
	virtual void PostRender() override;

protected: // make the class abstract
	explicit FadeableScene(float fadeTime = 0.5f, Color fadeColor = Color::Black);

private:
	struct Impl;
	std::unique_ptr<Impl> _pImpl;
};