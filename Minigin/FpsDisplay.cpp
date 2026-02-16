#include <format>
#include "FpsDisplay.h"
#include "Timer.h"

using namespace dae;

FpsDisplay::FpsDisplay()
{
	auto font{ ResourceManager::GetInstance().LoadFont( "Lingua.otf", 36 ) };
	const std::string fpsDisplayString{ std::format( "{:.1f} FPS", m_Fps ) };
	m_Text = TextObject( fpsDisplayString, font );
}

void FpsDisplay::Render() const
{
	m_Text.Render();
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

		m_Text.SetText( fpsDisplayString );
		m_Text.SetPosition( 0.f, 0.f );

		m_Text.Update();
	}
}
