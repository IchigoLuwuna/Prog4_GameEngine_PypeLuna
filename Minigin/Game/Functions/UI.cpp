#include "UI.h"

#include "Game/Components/FpsComponent.h"
#include "Game/Components/PixelTextComponent.h"
#include "Game/Components/ScoreComponent.h"
#include "Game/Components/ScoreDisplayComponent.h"
#include "Game/Components/ScrollingBGComponent.h"

std::unique_ptr<dae::GameObject> dae::functions::ui::MakeBackground()
{
	auto background{ std::make_unique<dae::GameObject>() };

	background->AddComponent<dae::ScrollingBGComponent>(
		"BG.png", 64.f, dae::ScrollingBGComponent::ScrollingDir::down );

	return background;
}

std::unique_ptr<dae::GameObject> dae::functions::ui::MakeFpsCounter()
{
	auto fps{ std::make_unique<dae::GameObject>() };

	const std::string typefacePath{ "Typeface.png" };
	const std::string typefaceMapping{ "0123456789abcdefghijklmnopqrstuvwxyz-%.!" };
	fps->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } )
		.SetIgnore( true );
	fps->AddComponent<dae::FpsComponent>();

	return fps;
}

std::unique_ptr<dae::GameObject> dae::functions::ui::MakeScoreboard( GameObject* pPlayer )
{
	auto scoreboard{ std::make_unique<dae::GameObject>() };

	const std::string typefacePath{ "Typeface.png" };
	const std::string typefaceMapping{ "0123456789abcdefghijklmnopqrstuvwxyz-%.!" };
	scoreboard->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } );
	scoreboard->AddComponent<dae::ScoreDisplayComponent>().SetSubjectScore(
		pPlayer->GetComponent<dae::ScoreComponent>() );

	return scoreboard;
}
