#include <ratio>
#include <stdexcept>
#include <iostream>
#include <thread>

#if WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Minigin.h"
#include "Engine/Input/InputManager.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Helpers/Timer.h"
#include "EventManager.h"
#include "ResourceManager.h"
#include "Engine/Patterns/ServiceLocator.h"
#include "Engine/Sound/SoundService.h"

#if USE_STEAMWORKS
#	if WIN32
#		pragma warning( push )
#		pragma warning( disable : 4996 )
#	endif
#	include <steam_api.h>
#	if WIN32
#		pragma warning( pop )
#	endif

std::unique_ptr<dae::steam::SteamAchievements> dae::Minigin::steamAchievements{};
#endif

dae::EventManager dae::Minigin::eventManager{};

SDL_Window* g_Window{};

void LogSDLVersion( const std::string& message, int major, int minor, int patch )
{
#if WIN32
	std::stringstream ss;
	ss << message << major << "." << minor << "." << patch << "\n";
	OutputDebugString( ss.str().c_str() );
#else
	std::cout << message << major << "." << minor << "." << patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#	include "emscripten.h"

void LoopCallback( void* arg )
{
	const float frameStartTimePoint{ dae::Timer::GetInstance().GetTotalElapsed() };
	dae::Timer::GetInstance().Lap();

	reinterpret_cast<dae::Minigin*>( arg )->RunOneFrame();

	constexpr float framerate{ 60.f };
	const float remainingSleepTime{ frameStartTimePoint + ( 1.f / framerate ) -
									dae::Timer::GetInstance().GetTotalElapsed() };
	std::this_thread::sleep_for( std::chrono::duration<float, std::ratio<1>>( remainingSleepTime ) );
}
#endif

// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion()
{
	LogSDLVersion( "Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION );
	int version = SDL_GetVersion();
	LogSDLVersion( "Linked with SDL ",
				   SDL_VERSIONNUM_MAJOR( version ),
				   SDL_VERSIONNUM_MINOR( version ),
				   SDL_VERSIONNUM_MICRO( version ) );
	// LogSDLVersion("Compiled with SDL_image ",SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION,
	// SDL_IMAGE_MICRO_VERSION); version = IMG_Version(); LogSDLVersion("Linked with SDL_image ",
	// SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	LogSDLVersion( "Compiled with SDL_ttf ", SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_MICRO_VERSION );
	version = TTF_Version();
	LogSDLVersion( "Linked with SDL_ttf ",
				   SDL_VERSIONNUM_MAJOR( version ),
				   SDL_VERSIONNUM_MINOR( version ),
				   SDL_VERSIONNUM_MICRO( version ) );
}

dae::Minigin::Minigin( const std::filesystem::path& dataPath )
{
	PrintSDLVersion();

#if USE_STEAMWORKS
	if ( !SteamAPI_Init() )
	{
		throw std::runtime_error( "Fatal Error - Steam must be running to play this Game (SteamAPI_init failed)" );
	}

	steamAchievements = std::make_unique<steam::SteamAchievements>( std::vector<steam::Achievement>( 0 ) );
#endif

	if ( !SDL_InitSubSystem( SDL_INIT_VIDEO | SDL_INIT_GAMEPAD ) )
	{
		SDL_Log( "Renderer error: %s", SDL_GetError() );
		throw std::runtime_error( std::string( "SDL_Init Error: " ) + SDL_GetError() );
	}

	g_Window = SDL_CreateWindow( "Programming 4 assignment", 1024, 576, SDL_WINDOW_OPENGL );
	if ( g_Window == nullptr )
	{
		throw std::runtime_error( std::string( "SDL_CreateWindow Error: " ) + SDL_GetError() );
	}

	Renderer::GetInstance().Init( g_Window );
	ResourceManager::GetInstance().Init( dataPath );
}

dae::Minigin::~Minigin()
{
#if USE_STEAMWORKS
	SteamAPI_Shutdown();
#endif

	// Clean up singletons, globals and statics
	eventManager.Destroy();
	SceneManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	Renderer::GetInstance().Destroy();
	ServiceLocator<SoundService>::GetInstance().RegisterService( nullptr );
	//
	SDL_DestroyWindow( g_Window );
	g_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run( const std::function<void()>& load )
{
	load();
#ifndef __EMSCRIPTEN__
	while ( !m_Quit )
	{
		const float frameStartTimePoint{ Timer::GetInstance().GetTotalElapsed() };
		Timer::GetInstance().Lap();

		RunOneFrame();

		constexpr float framerate{ 60.f };
		const float remainingSleepTime{ frameStartTimePoint + ( 1.f / framerate ) -
										Timer::GetInstance().GetTotalElapsed() };
		std::this_thread::sleep_for( std::chrono::duration<float, std::ratio<1>>( remainingSleepTime ) );
	}
#else
	emscripten_set_main_loop_arg( &LoopCallback, this, 0, true );
#endif
}

void dae::Minigin::RunOneFrame()
{
	m_Quit = !InputManager::GetInstance().ProcessInput();
#if USE_STEAMWORKS
	SteamAPI_RunCallbacks();
#endif
	SceneManager::GetInstance().Update();
	Renderer::GetInstance().Render();
	eventManager.ProcessEvents();
	SceneManager::GetInstance().CleanUpRemovableObjects();
	ResourceManager::GetInstance().UnloadUnusedResources();
}
