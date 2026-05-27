#include "SDLSoundServiceNoAsync.h"
#include <iostream>
#include <unordered_map>
#include <SDL3_mixer/SDL_mixer.h>
#include <Core.h>

class dae::SDLSoundServiceNoAsync::Impl
{
public:
	Impl();
	~Impl();

	void Play( const char* path, float volume );

private:
	MIX_Mixer* m_pMixer{};
	std::unordered_map<size_t, MIX_Audio*> m_AudioSamples{};

	MIX_Audio* LoadAudio( const char* path );
	void Playsound( MIX_Audio* audio, float volume );
};

dae::SDLSoundServiceNoAsync::SDLSoundServiceNoAsync()
	: m_pImpl( std::make_unique<Impl>() )
{
}
dae::SDLSoundServiceNoAsync::Impl::Impl()
{
	MIX_Init();
	m_pMixer = MIX_CreateMixerDevice( SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr );
}

dae::SDLSoundServiceNoAsync::~SDLSoundServiceNoAsync() = default;
dae::SDLSoundServiceNoAsync::Impl::~Impl()
{
	MIX_DestroyMixer( m_pMixer );
	MIX_Quit();
}

void dae::SDLSoundServiceNoAsync::Play( const char* path, float volume )
{
	m_pImpl->Play( path, volume );
}
void dae::SDLSoundServiceNoAsync::Impl::Play( const char* path, float )
{
	auto hash{ std::hash<std::string>()( path ) };
	if ( !m_AudioSamples.contains( hash ) )
	{
		m_AudioSamples.insert( { hash, LoadAudio( path ) } );
	}
}

MIX_Audio* dae::SDLSoundServiceNoAsync::Impl::LoadAudio( const char* path )
{
	auto fullPath{ ResourceManager::GetInstance().GetDataPath() / path };

	auto audio{ MIX_LoadAudio( m_pMixer, fullPath.string().c_str(), true ) };
	if ( !audio )
	{
		std::cout << SDL_GetError() << "\n";
		assert( false && "Loading audio failed, see above for SDL error" );
	}
	return audio;
}

void dae::SDLSoundServiceNoAsync::Impl::Playsound( MIX_Audio* audio, float )
{
	if ( !MIX_PlayAudio( m_pMixer, audio ) )
	{
		std::cout << SDL_GetError() << "\n";
		assert( false && "Playing audio failed, see above for SDL error" );
	}
}
