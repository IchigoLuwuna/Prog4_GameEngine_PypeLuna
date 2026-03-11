#include "Gamepad.h"
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_scancode.h>
#include <iostream>

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

	dae::Gamepad::Button RemapButtonFromSDL( SDL_GamepadButton in );
};

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
};

#ifdef __WINDOWS__
class dae::Gamepad::XInputImpl : public GamepadImpl
{
public:
private:
};
#endif

dae::Gamepad::Gamepad()
#if __WINDOWS__
	: m_pImpl{} // do this later
#else
	: m_pImpl{ std::make_unique<SDLImpl>() }
#endif
{
}

dae::Gamepad::~Gamepad() = default;

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

int dae::Gamepad::RemapButtonToKey( Button in )
{
	return static_cast<int>( in ) + SDL_SCANCODE_ENDCALL + 1;
}

void dae::Gamepad::AddGamepad()
{
	m_pImpl->AddGamepad();
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

void dae::Gamepad::RemoveGamepad()
{
	return m_pImpl->RemoveGamepad();
}
void dae::Gamepad::SDLImpl::RemoveGamepad()
{
	std::cout << "Removed Gamepad: " << SDL_GetGamepadName( m_pGamepad ) << "\n";
	SDL_CloseGamepad( m_pGamepad );
	m_pGamepad = nullptr;
}

void dae::Gamepad::UpdateGamepad()
{
	m_pImpl->UpdateGamepad();
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

dae::Gamepad::Button dae::Gamepad::GamepadImpl::RemapButtonFromSDL( SDL_GamepadButton in )
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

std::bitset<dae::Gamepad::maskBits> dae::Gamepad::GetMaskFromButtonID( Gamepad::Button in )
{
	return 1 << ( static_cast<uint16_t>( in ) );
}
