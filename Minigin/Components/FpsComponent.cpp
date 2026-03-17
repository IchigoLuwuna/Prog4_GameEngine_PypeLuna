#include "FpsComponent.h"
#include "Engine/Helpers/Timer.h"
#include "Engine/Patterns/GameObject.h"
#include "TextComponent.h"

dae::FpsComponent::FpsComponent( GameObject* pParent )
	: Component( pParent )
{
	if ( !m_pTextComponent )
	{
		m_pTextComponent = GetParent()->GetComponent<dae::TextComponent>();
		assert( m_pTextComponent && "FpsComponent requires parent to have a text component" );
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
