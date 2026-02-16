#include <format>
#include <iostream>
#include "FpsDisplay.h"
#include "Timer.h"

using namespace dae;

FpsDisplay::FpsDisplay()
{
	auto font{ ResourceManager::GetInstance().LoadFont( "Lingua.otf", 36 ) };
	const std::string fpsDisplayString{ std::format( "{:.1f} FPS", m_Fps ) };
	m_upText = std::make_unique<TextObject>( fpsDisplayString, font ); // Text object doesn't have move semantics
}

void FpsDisplay::Render() const
{
	m_upText->Render();
}

void FpsDisplay::Update()
{
	// Update
	constexpr float updateInterval{ 1.f };
	const float totalElapsed{ Timer::GetInstance().GetTotalElapsed() };
	if ( totalElapsed > m_LastUpdate + updateInterval )
	{
		m_LastUpdate = totalElapsed;
		m_Fps = 1.f / Timer::GetInstance().GetElapsed();

		// TODO:figure out a way to do this w/o doing a lot of dynamic allocation @runtime
		const std::string fpsDisplayString{ std::format( "{:.1f} FPS", m_Fps ) };
		std::cout << fpsDisplayString << "\n";

		m_upText->SetText( fpsDisplayString );
		m_upText->SetPosition( 0.f, 0.f );

		m_upText->Update();
	}
}
