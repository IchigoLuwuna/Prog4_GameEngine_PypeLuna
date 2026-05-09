#include "UI.h"
#include "Game/Components/FpsComponent.h"
#include "Game/Components/PixelTextComponent.h"

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
