#include "UI.h"
#include "Game/Context.h"

#include <Scene.h>
#include <Components.h>

#include "Game/Components/FpsComponent.h"
#include "Game/Components/PixelTextComponent.h"
#include "Game/Components/ScoreComponent.h"
#include "Game/Components/ScoreDisplayComponent.h"
#include "Game/Components/ScrollingBGComponent.h"

std::unique_ptr<dae::GameObject> dae::functions::ui::MakeBackground()
{
	auto background{ std::make_unique<dae::GameObject>( "bg"_hash ) };

	background->AddComponent<dae::ScrollingBGComponent>(
		"BG.png", 64.f, dae::ScrollingBGComponent::ScrollingDir::down );

#ifndef NDEBUG
	background->AddComponent<dae::DebugComponent>( "background" );
#endif

	return background;
}

std::unique_ptr<dae::GameObject> dae::functions::ui::MakeFpsCounter()
{
	auto fps{ std::make_unique<dae::GameObject>( "fps"_hash ) };

	const std::string typefacePath{ "Typeface.png" };
	const std::string typefaceMapping{ "0123456789abcdefghijklmnopqrstuvwxyz-%.!" };
	fps->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } )
		.SetIgnore( true );
	fps->AddComponent<dae::FpsComponent>();

#ifndef NDEBUG
	fps->AddComponent<dae::DebugComponent>( "fps" );
#endif

	return fps;
}

std::unique_ptr<dae::GameObject> dae::functions::ui::MakeScoreboard()
{
	auto scoreboard{ std::make_unique<dae::GameObject>( "scoreboard"_hash ) };

	const std::string typefacePath{ "Typeface.png" };
	const std::string typefaceMapping{ "0123456789abcdefghijklmnopqrstuvwxyz-%.!" };
	scoreboard->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } );
	scoreboard->AddComponent<dae::ScoreDisplayComponent>();
	auto* pPlayer{ SceneManager::GetInstance().GetScene( gameIdx ).GetByTag( "player"_hash ) };
	if ( pPlayer )
	{
		auto scoreComponent{ pPlayer->GetComponent<dae::ScoreComponent>() };
		if ( scoreComponent.Validate() )
		{
			scoreboard->GetComponent<dae::ScoreDisplayComponent>()->SetSubjectScore( scoreComponent );
		}
	}

#ifndef NDEBUG
	scoreboard->AddComponent<dae::DebugComponent>( "scoreboard" );
#endif

	return scoreboard;
}
