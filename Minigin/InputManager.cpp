#include "InputManager.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include "backends/imgui_impl_sdl3.h"

bool dae::InputManager::ProcessInput()
{
	UpdateKeyStates();
	m_Gamepad.UpdateGamepad();
	ProcessKeyboard();
	ProcessGamepad();

	SDL_Event e{};
	while ( SDL_PollEvent( &e ) )
	{
		switch ( e.type )
		{
		case SDL_EVENT_QUIT: {
			return false;
			break;
		}
		case SDL_EVENT_GAMEPAD_ADDED: {
			m_Gamepad.AddGamepad();
			break;
		}
		case SDL_EVENT_GAMEPAD_REMOVED: {
			m_Gamepad.RemoveGamepad();
			break;
		}
		}
		// etc...

		ImGui_ImplSDL3_ProcessEvent( &e );
	}

	return true;
}

void dae::InputManager::ClearBinding( int key, KeyState state )
{
	m_CommandBindings[key][static_cast<int>( state )] = nullptr; // I love RAII :D
}

void dae::InputManager::UpdateKeyStates()
{
	const auto pKeyboardState{ SDL_GetKeyboardState( nullptr ) };
	m_PreviousKeyStates = m_KeyStates;

	for ( int index{}; index < SDL_SCANCODE_COUNT; ++index ) // Raw array so we have to traverse it like peasants do
	{
		m_KeyStates.set( index, pKeyboardState[index] );
	}
}

void dae::InputManager::ProcessKeyboard()
{
	for ( size_t index{}; index < std::size( m_KeyStates ); ++index )
	{
		const auto key{ static_cast<SDL_Scancode>( index ) };
		const auto keyIsPressed{ m_KeyStates[index] };

		// On held
		if ( keyIsPressed )
		{
			const KeyState state{ KeyState::held };
			const auto& boundCommand{ m_CommandBindings[key][static_cast<int>( state )] };
			if ( boundCommand )
			{
				boundCommand->Execute();
			}
		}

		// Get change mask
		const auto changeMask{ m_KeyStates ^ m_PreviousKeyStates };

		// On down
		if ( keyIsPressed && changeMask[index] )
		{
			const KeyState state{ KeyState::down };
			const auto& boundCommand{ m_CommandBindings[key][static_cast<int>( state )] };
			if ( boundCommand )
			{
				boundCommand->Execute();
			}
		}

		// On up
		if ( !keyIsPressed && changeMask[index] )
		{
			const KeyState state{ KeyState::up };
			const auto& boundCommand{ m_CommandBindings[key][static_cast<int>( state )] };
			if ( boundCommand )
			{
				boundCommand->Execute();
			}
		}
	}
}

void dae::InputManager::ProcessGamepad()
{
	const auto controllerStateMask{ m_Gamepad.GetMask() };
	for ( Gamepad::Button button{}; button < Gamepad::Button::count;
		  button = static_cast<Gamepad::Button>( static_cast<int>( button ) + 1 ) )
	{
		const auto index{ static_cast<int>( button ) };
		const auto key{ static_cast<SDL_Scancode>( Gamepad::RemapButtonToKey( button ) ) };
		const auto buttonIsPressed{ controllerStateMask[index] };

		// On held
		if ( buttonIsPressed )
		{
			const KeyState state{ KeyState::held };
			const auto& boundCommand{ m_CommandBindings[key][static_cast<int>( state )] };
			if ( boundCommand )
			{
				boundCommand->Execute();
			}
		}

		// Get change mask
		auto changeMask{ controllerStateMask ^ m_Gamepad.GetPreviousMask() };

		// On down
		if ( buttonIsPressed && changeMask[index] )
		{
			const KeyState state{ KeyState::down };
			const auto& boundCommand{ m_CommandBindings[key][static_cast<int>( state )] };
			if ( boundCommand )
			{
				boundCommand->Execute();
			}
		}

		// On up
		if ( !buttonIsPressed && changeMask[index] )
		{
			const KeyState state{ KeyState::up };
			const auto& boundCommand{ m_CommandBindings[key][static_cast<int>( state )] };
			if ( boundCommand )
			{
				boundCommand->Execute();
			}
		}
	}
}
