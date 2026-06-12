#include "Gamepad.h"
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_gamepad.h>

#ifndef NDEBUG
#	include <iostream>
#endif

class dae::Gamepad::GamepadImpl
{
public:
	GamepadImpl();
	GamepadImpl( const GamepadImpl& ) = delete;
	GamepadImpl( GamepadImpl&& other );
	GamepadImpl& operator=( const GamepadImpl& ) = delete;
	GamepadImpl& operator=( GamepadImpl&& other );
	~GamepadImpl();

	void AddGamepad();
	void RemoveGamepad();

	std::pair<int16_t, int16_t> PollAxis( Gamepad::Axis axis );

	void UpdateGamepad();
	std::bitset<maskBits> GetMask();
	std::bitset<maskBits> GetPreviousMask();

private:
	SDL_Gamepad* m_pGamepad{};

	std::bitset<maskBits> m_ButtonMask{};
	std::bitset<maskBits> m_PreviousMask{};

	dae::Gamepad::Button RemapButtonFromSDL( SDL_GamepadButton in );
};

dae::Gamepad::Gamepad()
	: m_pImpl{ std::make_unique<GamepadImpl>() }
{
}
dae::Gamepad::GamepadImpl::GamepadImpl()
{
	AddGamepad();
}

dae::Gamepad::~Gamepad() = default;

dae::Gamepad::GamepadImpl::~GamepadImpl()
{
	SDL_CloseGamepad( m_pGamepad );
	m_pGamepad = nullptr;
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

int dae::Gamepad::RemapButtonToKey( Button in )
{
	return static_cast<int>( in ) + SDL_SCANCODE_ENDCALL + 1;
}

void dae::Gamepad::AddGamepad()
{
	m_pImpl->AddGamepad();
}
void dae::Gamepad::GamepadImpl::AddGamepad()
{
	if ( m_pGamepad ) // If gamepad is already connected and open
	{
		return;
	}
	if ( !SDL_HasGamepad() )
	{
#ifndef NDEBUG
		std::cout << "No gamepad detected\n";
#endif
		return;
	}
	int gamepadCount{};
	SDL_JoystickID* gamepads{ SDL_GetGamepads( &gamepadCount ) };

	// We just want Player 1
	m_pGamepad = SDL_OpenGamepad( gamepads[0] );

#ifndef NDEBUG
	std::cout << "Connected with Gamepad: " << SDL_GetGamepadName( m_pGamepad ) << "\n";
#endif
}

void dae::Gamepad::RemoveGamepad()
{
	return m_pImpl->RemoveGamepad();
}
void dae::Gamepad::GamepadImpl::RemoveGamepad()
{
#ifndef NDEBUG
	std::cout << "Removed Gamepad: " << SDL_GetGamepadName( m_pGamepad ) << "\n";
#endif
	SDL_CloseGamepad( m_pGamepad );
	m_pGamepad = nullptr;
}

std::pair<int16_t, int16_t> dae::Gamepad::PollAxis( Axis axis )
{
	return m_pImpl->PollAxis( axis );
}
std::pair<int16_t, int16_t> dae::Gamepad::GamepadImpl::PollAxis( Gamepad::Axis axis )
{
	SDL_GamepadAxis axisX{};
	SDL_GamepadAxis axisY{};

	switch ( axis )
	{
	case Axis::lStick: {
		axisX = SDL_GAMEPAD_AXIS_LEFTX;
		axisY = SDL_GAMEPAD_AXIS_LEFTY;
		break;
	}
	case Axis::rStick: {
		axisX = SDL_GAMEPAD_AXIS_RIGHTX;
		axisY = SDL_GAMEPAD_AXIS_RIGHTY;
		break;
	}
	case Axis::triggers: {
		axisX = SDL_GAMEPAD_AXIS_LEFT_TRIGGER;
		axisY = SDL_GAMEPAD_AXIS_RIGHT_TRIGGER;
		break;
	}
	default: {
		return {};
	}
	}

	int16_t x{ SDL_GetGamepadAxis( m_pGamepad, axisX ) };
	int16_t y{ SDL_GetGamepadAxis( m_pGamepad, axisY ) };
	return { x, y };
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

std::bitset<dae::Gamepad::maskBits> dae::Gamepad::GetMaskFromButtonID( Gamepad::Button in )
{
	return static_cast<std::bitset<maskBits>>( 1ll << ( static_cast<uint64_t>( in ) ) );
}

dae::Gamepad::Button dae::Gamepad::GamepadImpl::RemapButtonFromSDL( SDL_GamepadButton in )
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
