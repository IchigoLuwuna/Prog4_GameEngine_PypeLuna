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

void dae::DebugSoundService::StopAll()
{
	std::cout << "Stopping all sounds\n";
	m_pUnderlyingService->StopAll();
}

void dae::DebugSoundService::Mute()
{
	std::cout << "Muted audio\n";
	m_pUnderlyingService->Mute();
}
void dae::DebugSoundService::UnMute()
{
	std::cout << "Unmuted audio\n";
	m_pUnderlyingService->UnMute();
}
