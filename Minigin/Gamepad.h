#ifndef GAMEPAD_H
#define GAMEPAD_H
#include <memory>
#include <bitset>

namespace dae
{
class Gamepad final
{
public:
	enum class Button : int16_t
	{
		invalid = -1,
		up,
		down,
		left,
		right,
		start,
		select,
		l3,
		r3,
		l1,
		r1,
		// Platform agnostic face buttons
		south, // (XBox A; Nintendo B; PS Cross)
		east,  // (XBox B; Nintendo A; PS Circle)
		west,  // (XBox X; Nintendo Y; PS Square)
		north, // (XBox Y; Nintendo X; PS Triangle)
		count,
	};
	static constexpr int maskBits{ static_cast<int>( Gamepad::Button::count ) }; // a bit easier to read than constantly
	// casting

	Gamepad();
	~Gamepad();

	void AddGamepad();
	void RemoveGamepad();

	void UpdateGamepad();
	std::bitset<maskBits> GetMask();
	std::bitset<maskBits> GetPreviousMask();

	static int RemapButtonToKey( Button in );
	static std::bitset<maskBits> GetMaskFromButtonID( Gamepad::Button in );

private:
	class GamepadImpl;
	class SDLImpl;
	class XInputImpl;
	std::unique_ptr<GamepadImpl> m_pImpl{};
};
} // namespace dae
#endif
