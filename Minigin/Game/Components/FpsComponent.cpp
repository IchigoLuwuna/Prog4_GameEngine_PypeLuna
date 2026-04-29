#include "FpsComponent.h"
#include "TextComponent.h"
#include <Helpers.h>

dae::FpsComponent::FpsComponent( GameObject* pParent )
	: Component( pParent )
{
	if ( !m_pTextComponent.Validate() )
	{
		m_pTextComponent = GetParent()->GetComponent<TextComponent>();
		assert( m_pTextComponent.Validate() && "FpsComponent requires parent to have a text component" );
	}

	UpdateText();
}

void dae::FpsComponent::Update()
{
	constexpr float updateInterval{ 0.5f };
	const float totalElapsed{ Timer::GetInstance().GetTotalElapsed() };
	if ( totalElapsed > m_LastUpdate + updateInterval )
	{
		UpdateText();
	}
}

void dae::FpsComponent::UpdateText()
{
	const float totalElapsed{ Timer::GetInstance().GetTotalElapsed() };

	m_LastUpdate = totalElapsed;
	m_Fps = 1.f / Timer::GetInstance().GetElapsed();

	const std::string fpsDisplayString{ std::format( "{:.1f} FPS", m_Fps ) };

	m_pTextComponent->SetText( fpsDisplayString );
}
