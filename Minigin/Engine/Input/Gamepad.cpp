#include "Gamepad.h"
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_scancode.h>
#include <iostream>
#ifdef _WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <XInput.h>
#endif

class dae::Gamepad::GamepadImpl
{
public:
	virtual ~GamepadImpl() = default;

	virtual void AddGamepad() = 0;
	virtual void RemoveGamepad() = 0;
	virtual void UpdateGamepad() = 0;
	std::bitset<maskBits> GetMask();
	std::bitset<maskBits> GetPreviousMask();

protected:
	std::bitset<maskBits> m_ButtonMask{};
	std::bitset<maskBits> m_PreviousMask{};
};

#ifndef _WIN32
class dae::Gamepad::SDLImpl : public GamepadImpl
{
public:
	SDLImpl();
	virtual ~SDLImpl();

	SDLImpl( const SDLImpl& ) = delete;
	SDLImpl& operator=( const SDLImpl& ) = delete;
	SDLImpl( SDLImpl&& other );
	SDLImpl& operator=( SDLImpl&& other );

	virtual void AddGamepad() override;
	virtual void RemoveGamepad() override;
	virtual void UpdateGamepad() override;

private:
	SDL_Gamepad* m_pGamepad{};

	dae::Gamepad::Button RemapButtonFromSDL( SDL_GamepadButton in );
};
#else
class dae::Gamepad::XInputImpl : public GamepadImpl
{
public:
	virtual ~XInputImpl() = default;

	virtual void AddGamepad() override
	{
	} // XInput doesn't need manual gamepad management
	virtual void RemoveGamepad() override
	{
	}
	virtual void UpdateGamepad() override;

private:
	int m_ControllerIndex{};

	dae::Gamepad::Button RemapButtonFromXInput( WORD in );
};
#endif

dae::Gamepad::Gamepad()
#ifdef _WIN32
	: m_pImpl{ std::make_unique<XInputImpl>() }
#else
	: m_pImpl{ std::make_unique<SDLImpl>() }
#endif
{
}

dae::Gamepad::~Gamepad() = default;

int dae::Gamepad::RemapButtonToKey( Button in )
{
	return static_cast<int>( in ) + SDL_SCANCODE_ENDCALL + 1;
}

void dae::Gamepad::AddGamepad()
{
	m_pImpl->AddGamepad();
}
void dae::Gamepad::RemoveGamepad()
{
	return m_pImpl->RemoveGamepad();
}

void dae::Gamepad::UpdateGamepad()
{
	m_pImpl->UpdateGamepad();
}

std::bitset<dae::Gamepad::maskBits> dae::Gamepad::GetMask()
{
	return m_pImpl->GetMask();
}
std::bitset<dae::Gamepad::maskBits> dae::Gamepad::GamepadImpl::GetMask()
{
	return m_ButtonMask;
}

std::bitset<dae::Gamepad::maskBits> dae::Gamepad::GetPreviousMask()
{
	return m_pImpl->GetPreviousMask();
}
std::bitset<dae::Gamepad::maskBits> dae::Gamepad::GamepadImpl::GetPreviousMask()
{
	return m_PreviousMask;
}

std::bitset<dae::Gamepad::maskBits> dae::Gamepad::GetMaskFromButtonID( Gamepad::Button in )
{
	return static_cast<std::bitset<maskBits>>( 1ll << ( static_cast<uint64_t>( in ) ) );
}

#ifndef _WIN32
// SDL
dae::Gamepad::SDLImpl::SDLImpl()
{
	AddGamepad();
}

dae::Gamepad::SDLImpl::~SDLImpl()
{
	SDL_CloseGamepad( m_pGamepad );
	m_pGamepad = nullptr;
}

dae::Gamepad::SDLImpl::SDLImpl( SDLImpl&& other )
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
dae::Gamepad::SDLImpl& dae::Gamepad::SDLImpl::operator=( SDLImpl&& other )
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

void dae::Gamepad::SDLImpl::AddGamepad()
{
	if ( m_pGamepad ) // If gamepad is already connected and open
	{
		return;
	}
	if ( !SDL_HasGamepad() )
	{
		std::cout << "No gamepad detected\n";
		return;
	}
	int gamepadCount{};
	SDL_JoystickID* gamepads{ SDL_GetGamepads( &gamepadCount ) };

	// We just want Player 1
	m_pGamepad = SDL_OpenGamepad( gamepads[0] );

	std::cout << "Connected with Gamepad: " << SDL_GetGamepadName( m_pGamepad ) << "\n";
}

void dae::Gamepad::SDLImpl::RemoveGamepad()
{
	std::cout << "Removed Gamepad: " << SDL_GetGamepadName( m_pGamepad ) << "\n";
	SDL_CloseGamepad( m_pGamepad );
	m_pGamepad = nullptr;
}

void dae::Gamepad::SDLImpl::UpdateGamepad()
{
	if ( !m_pGamepad )
	{
		return;
	}

	m_PreviousMask = m_ButtonMask;
	m_ButtonMask = 0;

	SDL_UpdateGamepads();

	for ( SDL_GamepadButton sdlButton{}; sdlButton < SDL_GAMEPAD_BUTTON_COUNT;
		  sdlButton = static_cast<SDL_GamepadButton>( static_cast<int>( sdlButton ) + 1 ) ) // I love static casts
	{
		if ( bool buttonState{ SDL_GetGamepadButton( m_pGamepad, sdlButton ) }; buttonState )
		{
			auto button{ RemapButtonFromSDL( sdlButton ) };
			if ( button != Gamepad::Button::invalid )
			{
				m_ButtonMask |= GetMaskFromButtonID( button );
			}
		}
	}
}

dae::Gamepad::Button dae::Gamepad::SDLImpl::RemapButtonFromSDL( SDL_GamepadButton in )
{
	switch ( in )
	{
	case SDL_GAMEPAD_BUTTON_DPAD_UP:
		return Button::up;
	case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
		return Button::down;
	case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
		return Button::left;
	case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
		return Button::right;

	case SDL_GAMEPAD_BUTTON_START:
		return Button::start;
	case SDL_GAMEPAD_BUTTON_BACK:
		return Button::select;

	case SDL_GAMEPAD_BUTTON_LEFT_STICK:
		return Button::l3;
	case SDL_GAMEPAD_BUTTON_RIGHT_STICK:
		return Button::r3;

	case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
		return Button::l1;
	case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
		return Button::r1;

	case SDL_GAMEPAD_BUTTON_SOUTH:
		return Button::south;
	case SDL_GAMEPAD_BUTTON_EAST:
		return Button::east;
	case SDL_GAMEPAD_BUTTON_WEST:
		return Button::west;
	case SDL_GAMEPAD_BUTTON_NORTH:
		return Button::north;

	default:
		return Button::invalid;
	}
}

#else
// XInput
// I only have a DualSense controller so I have absolutely no way of verifying if this works :3
// Formatting might also look different because I don't have my auto formatter on windows D:
void dae::Gamepad::XInputImpl::UpdateGamepad()
{
	XINPUT_STATE state{};
	ZeroMemory( &state, sizeof( XINPUT_STATE ) );
	XInputGetState( static_cast<DWORD>( m_ControllerIndex ), &state );

	m_PreviousMask = m_ButtonMask;
	m_ButtonMask = state.Gamepad.wButtons;
}

dae::Gamepad::Button dae::Gamepad::XInputImpl::RemapButtonFromXInput( WORD in )
{
	// Unused because XInput's button mask layout alligns with the generic mask layout :D
	// Still implemented for the sake of completeness
	switch ( in )
	{
	case XINPUT_GAMEPAD_DPAD_UP:
		return Button::up;
	case XINPUT_GAMEPAD_DPAD_DOWN:
		return Button::down;
	case XINPUT_GAMEPAD_DPAD_LEFT:
		return Button::left;
	case XINPUT_GAMEPAD_DPAD_RIGHT:
		return Button::right;

	case XINPUT_GAMEPAD_START:
		return Button::start;
	case XINPUT_GAMEPAD_BACK:
		return Button::select;

	case XINPUT_GAMEPAD_LEFT_THUMB:
		return Button::l3;
	case XINPUT_GAMEPAD_RIGHT_THUMB:
		return Button::r3;

	case XINPUT_GAMEPAD_LEFT_SHOULDER:
		return Button::l1;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER:
		return Button::r1;

	case XINPUT_GAMEPAD_A:
		return Button::south;
	case XINPUT_GAMEPAD_B:
		return Button::east;
	case XINPUT_GAMEPAD_X:
		return Button::west;
	case XINPUT_GAMEPAD_Y:
		return Button::north;

	default:
		return Button::invalid;
	}
}
#endif
