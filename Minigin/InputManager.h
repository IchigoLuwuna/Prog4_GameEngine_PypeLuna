#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "Singleton.h"
#include <unordered_map>
#include <SDL3/SDL_scancode.h>
#include <bitset>
#include <memory>
#include "Command.h"

namespace dae
{
class InputManager final : public Singleton<InputManager>
{
public:
	bool ProcessInput();

	template <typename CommandType, SDL_Scancode key, typename... Types>
		requires std::derived_from<CommandType, Command>
	void BindCommandToKey( const Types&... args )
	{
		m_CommandBindings.insert( { key, std::make_unique<CommandType>( args... ) } );
	}

private:
	enum class KeyState : uint8_t
	{
		none,
		held,
		down,
		up,
	};

	struct KeyBind
	{
		SDL_Scancode key{};
		KeyState state{};
	};

	std::bitset<SDL_SCANCODE_COUNT> m_PreviousKeyStates{};
	std::unordered_map<SDL_Scancode, std::unique_ptr<Command>> m_CommandBindings{};

	void UpdatePreviousKeyStates();
	void ProcessKeyboard();
};
} // namespace dae
#endif
