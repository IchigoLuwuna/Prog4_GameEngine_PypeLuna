#include "Gamepad.h"
#include <bitset>
#include <iostream>
#include <SDL3/SDL_gamepad.h>

class dae::Gamepad::GamepadImpl
{
public:
	GamepadImpl();
	~GamepadImpl();

	GamepadImpl( const GamepadImpl& ) = delete;
	GamepadImpl& operator=( const GamepadImpl& ) = delete;
	GamepadImpl( GamepadImpl&& other );
	GamepadImpl& operator=( GamepadImpl&& other );

	void UpdateGamepad();

private:
	SDL_Gamepad* m_pGamepad{};
	std::bitset<static_cast<int>( Gamepad::Button::count )> m_ButtonMask{};
	std::bitset<static_cast<int>( Gamepad::Button::count )> m_PreviousMask{};

	dae::Gamepad::Button RemapButton( SDL_GamepadButton in );
};

dae::Gamepad::Gamepad()
#if __WINDOWS__
	: m_pImpl{} // do this later
#else
	: m_pImpl{ std::make_unique<GamepadImpl>() }
#endif
{
}

dae::Gamepad::~Gamepad() = default;

dae::Gamepad::GamepadImpl::GamepadImpl()
{
	if ( !SDL_HasGamepad() )
	{
		std::cout << "No gamepad detected\n";
		return;
	}
	int gamepadCount{};
	SDL_JoystickID* gamepads{ SDL_GetGamepads( &gamepadCount ) };

	// We just want Player 1
	m_pGamepad = SDL_OpenGamepad( gamepads[0] );
}
dae::Gamepad::GamepadImpl::~GamepadImpl()
{
	SDL_CloseGamepad( m_pGamepad );
}

dae::Gamepad::GamepadImpl::GamepadImpl( GamepadImpl&& other )
{
	if ( this == &other )
	{
		return;
	}

	m_pGamepad = other.m_pGamepad;
	other.m_pGamepad = nullptr;

	m_ButtonMask = other.m_ButtonMask;
	m_PreviousMask = other.m_PreviousMask;
}
dae::Gamepad::GamepadImpl& dae::Gamepad::GamepadImpl::operator=( GamepadImpl&& other )
{
	if ( this == &other )
	{
		return *this;
	}

	m_pGamepad = other.m_pGamepad;
	other.m_pGamepad = nullptr;

	m_ButtonMask = other.m_ButtonMask;
	m_PreviousMask = other.m_PreviousMask;

	return *this;
}

void dae::Gamepad::UpdateGamepad()
{
	m_pImpl->UpdateGamepad();
}
void dae::Gamepad::GamepadImpl::UpdateGamepad()
{
	if ( !m_pGamepad )
	{
		return;
	}

	m_PreviousMask = m_ButtonMask;
	m_ButtonMask = 0;

	SDL_UpdateGamepads();

	for ( SDL_GamepadButton button{}; button < SDL_GAMEPAD_BUTTON_COUNT;
		  button = static_cast<SDL_GamepadButton>( static_cast<int>( button ) + 1 ) ) // I love static casts
	{
		if ( bool buttonState{ SDL_GetGamepadButton( m_pGamepad, button ) }; buttonState )
		{
			auto buttonMask{ RemapButton( button ) };
			if ( buttonMask != Gamepad::Button::invalid )
			{
				std::cout << std::hex << static_cast<int>( buttonMask ) << "\n";
				m_ButtonMask |= static_cast<uint16_t>( buttonMask );
			}
		}
	}
}

dae::Gamepad::Button dae::Gamepad::GamepadImpl::RemapButton( SDL_GamepadButton in )
{
	switch ( in )
	{
	case SDL_GAMEPAD_BUTTON_DPAD_UP:
		return Gamepad::Button::up;
	case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
		return Gamepad::Button::down;
	case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
		return Gamepad::Button::left;
	case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
		return Gamepad::Button::right;

	case SDL_GAMEPAD_BUTTON_START:
		return Gamepad::Button::start;
	case SDL_GAMEPAD_BUTTON_BACK:
		return Gamepad::Button::select;

	case SDL_GAMEPAD_BUTTON_LEFT_STICK:
		return Gamepad::Button::l3;
	case SDL_GAMEPAD_BUTTON_RIGHT_STICK:
		return Gamepad::Button::r3;

	case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
		return Gamepad::Button::l1;
	case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
		return Gamepad::Button::r1;

	case SDL_GAMEPAD_BUTTON_SOUTH:
		return Gamepad::Button::south;
	case SDL_GAMEPAD_BUTTON_EAST:
		return Gamepad::Button::east;
	case SDL_GAMEPAD_BUTTON_WEST:
		return Gamepad::Button::west;
	case SDL_GAMEPAD_BUTTON_NORTH:
		return Gamepad::Button::north;

	default:
		return Gamepad::Button::invalid;
	}
}
