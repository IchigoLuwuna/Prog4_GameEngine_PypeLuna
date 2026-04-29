#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <SDL3/SDL_scancode.h>
#include <bitset>
#include <memory>
#include <array>
#include "Gamepad.h"
#include "Patterns/Singleton.h"
#include "Patterns/Command.h"

namespace dae
{
class InputManager final : public Singleton<InputManager>
{
public:
	enum class KeyState : uint8_t
	{
		held,
		down,
		up,
		count,
	};
	static constexpr int KeyStateCount{ static_cast<int>( KeyState::count ) };

	bool ProcessInput();

	template <typename CommandType, typename... Args>
		requires std::derived_from<CommandType, Command> && requires( Args... args ) { CommandType( args... ); }
	void BindCommand( int key, KeyState state, const Args&... args )
	{
		auto command{ std::make_unique<CommandType>( args... ) };
		m_CommandBindings[key][static_cast<int>( state )] = std::move( command );
	}
	void ClearBinding( int key, KeyState state );

private:
	std::bitset<SDL_SCANCODE_COUNT> m_KeyStates{};
	std::bitset<SDL_SCANCODE_COUNT> m_PreviousKeyStates{};
	std::array<std::array<std::unique_ptr<Command>, KeyStateCount>, SDL_SCANCODE_COUNT> m_CommandBindings{};
	Gamepad m_Gamepad{};

	void UpdateKeyStates();
	void ProcessKeyboard();
	void ProcessGamepad();
};
} // namespace dae
#endif
