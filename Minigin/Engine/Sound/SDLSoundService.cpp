#include "SDLSoundService.h"
#include <cassert>
#include <condition_variable>
#include <iostream>
#include <queue>
#include <thread>
#include <unordered_map>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3_mixer/SDL_mixer.h>

// SOURCES: https://wiki.libsdl.org/SDL3_mixer/CategorySDLMixer
// Thank you Mr. and Mr. SDL

class dae::SDLSoundService::Impl
{
public:
	Impl();
	~Impl();

	void Play( const char* path, float volume );

private:
	MIX_Mixer* m_pMixer{};
	std::queue<std::pair<const char*, float>> m_RequestQueue{};
	std::unordered_map<size_t, MIX_Audio*> m_AudioSamples{};

	// Threading
	std::jthread m_EventHandlerThread{};
	std::mutex m_EventQueueMutex{};
	std::condition_variable m_CV{};
	std::stop_source m_StopSrc{}; // Usually I would let jthread implicetely handle stop tokens but emscripten forced my
								  // hand by not liking that :(

	void HandleRequests( std::stop_token stopToken );
	MIX_Audio* LoadAudio( const char* path );
	void Playsound( MIX_Audio* audio, float volume );
};

dae::SDLSoundService::SDLSoundService()
	: m_pImpl( std::make_unique<Impl>() )
{
}
dae::SDLSoundService::Impl::Impl()
{
	MIX_Init(); // Can be safely called multiple times so no checks are needed
	m_pMixer = MIX_CreateMixerDevice( SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr );

	m_EventHandlerThread = std::jthread( &Impl::HandleRequests, this, m_StopSrc.get_token() );
}

dae::SDLSoundService::~SDLSoundService() = default;
dae::SDLSoundService::Impl::~Impl()
{
	std::unique_lock lock{ m_EventQueueMutex }; // Event handler might still be processing requests

	for ( auto& audio : m_AudioSamples )
	{
		if ( audio.second )
		{
			MIX_DestroyAudio( audio.second );
		}
	}

	m_StopSrc.request_stop();
	m_CV.notify_all(); // Event thread is waiting for a notification, so we give it one

	MIX_Quit();
}

void dae::SDLSoundService::Play( const char* path, const float volume )
{
	m_pImpl->Play( path, volume );
}
void dae::SDLSoundService::Impl::Play( const char* path, const float volume )
{
	std::unique_lock lock{ m_EventQueueMutex };
	m_RequestQueue.push( { path, volume } );
	m_CV.notify_all();
}

void dae::SDLSoundService::Impl::HandleRequests( std::stop_token stopToken )
{
	while ( !stopToken.stop_requested() )
	{
		// Main loop
		std::unique_lock lock{ m_EventQueueMutex };
		m_CV.wait( lock, [this, stopToken] {
			return !m_RequestQueue.empty() || stopToken.stop_requested();
		} ); // Wait for wakeup
		while ( !m_RequestQueue.empty() )
		{
			// Queue dependent code [LOCKED]
			auto path{ m_RequestQueue.back().first };
			auto volume{ m_RequestQueue.back().second };
			m_RequestQueue.pop();
			lock.unlock();
			//

			// Queue independent code [UNLOCKED]
			auto hash{ std::hash<std::string>()( path ) };
			if ( !m_AudioSamples.contains( hash ) )
			{
				m_AudioSamples.insert( { hash, LoadAudio( path ) } );
			}
			Playsound( m_AudioSamples[hash], volume );
			//

			lock.lock();
		}
	}
}

MIX_Audio* dae::SDLSoundService::Impl::LoadAudio( const char* path )
{
	auto audio{ MIX_LoadAudio( m_pMixer, path, true ) };
	if ( !audio )
	{
		std::cout << SDL_GetError() << "\n";
		assert( false && "Loading audio failed, see above for SDL error" );
	}
	return audio;
}

void dae::SDLSoundService::Impl::Playsound( MIX_Audio* audio, float )
{
	if ( !MIX_PlayAudio( m_pMixer, audio ) )
	{
		std::cout << SDL_GetError() << "\n";
		assert( false && "Playing audio failed, see above for SDL error" );
	}
}
