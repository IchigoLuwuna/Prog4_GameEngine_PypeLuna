#include "FpsComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Timer.h"

void dae::FpsComponent::Update()
{
	constexpr float updateInterval{ 0.5f };
	const float totalElapsed{ Timer::GetInstance().GetTotalElapsed() };
	if ( totalElapsed > m_LastUpdate + updateInterval )
	{
		m_LastUpdate = totalElapsed;
		m_Fps = 1.f / Timer::GetInstance().GetElapsed();

		const std::string fpsDisplayString{ std::format( "{:.1f} FPS", m_Fps ) };

		GetParent()->GetComponent<TextComponent>()->SetText( fpsDisplayString );
	}
}
