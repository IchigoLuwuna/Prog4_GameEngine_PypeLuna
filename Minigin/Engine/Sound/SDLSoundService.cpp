#include "SDLSoundService.h"
#include <condition_variable>
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
	std::vector<std::pair<const char*, float>> m_RequestQueue{};
	std::unordered_map<uint32_t, MIX_Audio*> m_AudioSamples{};

	// Threading
	std::jthread m_EventHandlerThread{};
	std::mutex m_Mutex{};
	std::condition_variable m_CV{};

	void HandleEvents( std::stop_token stopToken );
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

	// TODO:
	m_EventHandlerThread = std::jthread( &Impl::HandleEvents, this );
}

dae::SDLSoundService::~SDLSoundService() = default;
dae::SDLSoundService::Impl::~Impl()
{
	std::unique_lock lock{ m_Mutex }; // Event handler might still be processing requests

	m_RequestQueue.clear();

	for ( auto& audio : m_AudioSamples )
	{
		if ( audio.second )
		{
			MIX_DestroyAudio( audio.second );
		}
	}

	// TODO:This prevents a deadlock, but it's ugly, try to make it better :3
	m_EventHandlerThread.request_stop();
	m_CV.notify_all(); // Event thread is waiting for a notification, so we give it one

	MIX_Quit();
}

void dae::SDLSoundService::Play( const char* path, const float volume )
{
	m_pImpl->Play( path, volume );
}
void dae::SDLSoundService::Impl::Play( const char* path, const float volume )
{
	std::unique_lock lock{ m_Mutex };
	m_RequestQueue.push_back( { path, volume } );
	m_CV.notify_all();
}

void dae::SDLSoundService::Impl::HandleEvents( std::stop_token stopToken )
{
	while ( !stopToken.stop_requested() )
	{
		// Main loop
		std::unique_lock lock{ m_Mutex };
		m_CV.wait( lock, [this, stopToken] {
			return !m_RequestQueue.empty() || stopToken.stop_requested();
		} ); // Wait for wakeup
		for ( auto& request : m_RequestQueue )
		{
			const auto& path{ request.first };
			auto volume{ request.second };
			auto hash{ std::hash<std::string>()( path ) };
			if ( !m_AudioSamples.contains( hash ) )
			{
				m_AudioSamples.insert( { hash, LoadAudio( path ) } );
			}
			Playsound( m_AudioSamples[hash], volume );
		}
		m_RequestQueue.clear();
	}
}

MIX_Audio* dae::SDLSoundService::Impl::LoadAudio( const char* path )
{
	return MIX_LoadAudio( m_pMixer, path, true );
}

// TODO:Add volume
void dae::SDLSoundService::Impl::Playsound( MIX_Audio* audio, float )
{
	MIX_PlayAudio( m_pMixer, audio );
}
