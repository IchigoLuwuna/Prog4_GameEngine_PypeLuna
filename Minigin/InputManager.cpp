#include "InputManager.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include "backends/imgui_impl_sdl3.h"

bool dae::InputManager::ProcessInput()
{
	UpdatePreviousKeyStates();
	ProcessKeyboard();

	SDL_Event e{};
	while ( SDL_PollEvent( &e ) )
	{
		if ( e.type == SDL_EVENT_QUIT )
		{
			return false;
		}
		if ( e.type == SDL_EVENT_KEY_DOWN )
		{
		}
		if ( e.type == SDL_EVENT_MOUSE_BUTTON_DOWN )
		{
		}
		// etc...

		ImGui_ImplSDL3_ProcessEvent( &e );
	}

	return true;
}

void dae::InputManager::UpdatePreviousKeyStates()
{
	auto pKeyboardState{ SDL_GetKeyboardState( nullptr ) };

	for ( int index{}; index < SDL_SCANCODE_COUNT; ++index ) // Raw array so we have to traverse it like peasants do
	{
		m_PreviousKeyStates[index] = pKeyboardState[index];
	}
}

void dae::InputManager::ProcessKeyboard()
{
	int numkeys{};
	auto pKeyboardState{ SDL_GetKeyboardState( &numkeys ) };

	for ( int index{}; index < numkeys; ++index )
	{
		if ( pKeyboardState[index] )
		{
			auto key{ static_cast<SDL_Scancode>( index ) };
			if ( m_CommandBindings.contains( key ) )
			{
				m_CommandBindings[key]->Execute();
			}
		}
	}
}
