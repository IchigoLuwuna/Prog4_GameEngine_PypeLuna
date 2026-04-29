#include "ReactiveSoundComponent.h"
#include <Core.h>
#include <Sound.h>

dae::ReactiveSoundComponent::ReactiveSoundComponent( dae::GameObject* pParent )
	: Component( pParent )
	, m_Subscription( this, std::bind( &ReactiveSoundComponent::HandleEvent, this, std::placeholders::_1 ) )
{
}

dae::ReactiveSoundComponent::~ReactiveSoundComponent()
{
}

dae::ReactiveSoundComponent& dae::ReactiveSoundComponent::AddSound( const ReactiveSoundInfo& info )
{
	m_ReactiveSounds.push_back( info );
	return *this;
}

dae::ReactiveSoundComponent& dae::ReactiveSoundComponent::AddSound( ReactiveSoundInfo&& info )
{
	m_ReactiveSounds.push_back( std::move( info ) );
	return *this;
}

void dae::ReactiveSoundComponent::HandleEvent( Event& event )
{
	for ( const auto& reactiveSound : m_ReactiveSounds )
	{
		if ( reactiveSound.eventHash == event.eventHash &&
			 ( reactiveSound.pSubject == event.pData || !reactiveSound.pSubject ) )
		{
			ServiceLocator<SoundService>::GetInstance().GetService().Play( reactiveSound.path.c_str(), 1.f );
		}
	}
}
