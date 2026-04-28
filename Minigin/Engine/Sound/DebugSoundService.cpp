#include "DebugSoundService.h"
#include <iostream>

dae::DebugSoundService::DebugSoundService( std::unique_ptr<SoundService>&& pSoundService )
	: m_pUnderlyingService( std::move( pSoundService ) )
{
}

void dae::DebugSoundService::Play( const char* path, float volume )
{
	std::cout << "Playing sound " << path << " at " << volume << "x volume\n";
	m_pUnderlyingService->Play( path, volume );
}
