#include "FpsComponent.h"
#include "Engine/Helpers/Timer.h"
#include "Engine/Patterns/GameObject.h"
#include "TextComponent.h"

void dae::FpsComponent::Update()
{
	if ( !m_pTextComponent )
	{
		m_pTextComponent = GetParent()->GetComponent<dae::TextComponent>();
		assert( m_pTextComponent && "FpsComponent requires parent to have a text component" );
	}

	constexpr float updateInterval{ 0.5f };
	const float totalElapsed{ Timer::GetInstance().GetTotalElapsed() };
	if ( totalElapsed > m_LastUpdate + updateInterval )
	{
		m_LastUpdate = totalElapsed;
		m_Fps = 1.f / Timer::GetInstance().GetElapsed();

		const std::string fpsDisplayString{ std::format( "{:.1f} FPS", m_Fps ) };

		m_pTextComponent->SetText( fpsDisplayString );
	}
}
