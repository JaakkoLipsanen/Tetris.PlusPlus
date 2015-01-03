#include <Scenes/FadeableScene.h>

#include <Engine/Time.h>
#include <Engine/ISceneManager.h>
#include <Engine/Game.h>
#include <Diagnostics/Ensure.h>

#include <Graphics/SpriteBatch.h>
#include <Graphics/TextureHelper.h>
#include <Engine/Screen.h>

enum class FadeState
{
	// the int value specifies the "direction" where the state fades to
	FadingIn = -1,
	Normal = 0,
	FadingOut = 1,
};

struct FadeableScene::Impl
{
	FadeableScene::Impl(float fadeTime, Color fadeColor) :
		 FadeState(FadeState::FadingIn), FadeAlpha(static_cast<int>(1)), FadeTime(fadeTime), FadeColor(fadeColor)
	{
	}

	std::unique_ptr<Scene> SceneToLoad;
	FadeState FadeState;
	float FadeAlpha;

	const float FadeTime;
	const Color FadeColor;
};

FadeableScene::FadeableScene(float fadeTime, Color fadeColor) :
	 _pImpl(new FadeableScene::Impl(fadeTime, fadeColor))
{
}
FadeableScene::~FadeableScene() = default;

void FadeableScene::PostUpdate()
{
	if (_pImpl->FadeState != FadeState::Normal)
	{
		_pImpl->FadeAlpha += Time::GetDeltaTime() / _pImpl->FadeTime * static_cast<int>(_pImpl->FadeState);
		if (_pImpl->FadeState == FadeState::FadingIn)
		{
			if (_pImpl->FadeAlpha <= 0)
			{
				_pImpl->FadeAlpha = 0;
				_pImpl->FadeState = FadeState::Normal;
			}
		}
		else if (_pImpl->FadeState == FadeState::FadingOut)
		{
			if (_pImpl->FadeAlpha >= 1)
			{
				this->GetSceneManager().LoadScene(std::move(_pImpl->SceneToLoad));
			}
		}
	}
}

void FadeableScene::PostRender()
{
	if (_pImpl->FadeAlpha > 0)
	{
		/* TODO!!! I REALLY NEED TO HAVE A "GLOBAL" BLANKPIXEL AND SPRITEBATCH... IT SUCKS TO ALWAYS HAVE TO MAKE NEW ONE. SO YEAH, IMMA GONNA REMAKE THESE EVERY FRAME UNTIL I MAKE THOSE THING */
		SpriteBatch spriteBatch(this->GetGame().GetGraphicsContext());
		auto blankPixel = TextureHelper::CreateBlankTexture(this->GetGame().GetGraphicsContext());

		spriteBatch.Begin();
		spriteBatch.Draw(*blankPixel, Vector2f::Zero, _pImpl->FadeColor * _pImpl->FadeAlpha, 0.0f, Screen::GetSize().ToVector2i());
		spriteBatch.End();
	}
}

void FadeableScene::ChangeScene(std::unique_ptr<Scene> sceneToLoad)
{
	// another scene is already loading
	if (_pImpl->SceneToLoad.get() != nullptr)
	{
		return;
	}

	Ensure::NotNull(sceneToLoad.get());
	_pImpl->SceneToLoad.reset(sceneToLoad.release());
	_pImpl->FadeState = FadeState::FadingOut;
}