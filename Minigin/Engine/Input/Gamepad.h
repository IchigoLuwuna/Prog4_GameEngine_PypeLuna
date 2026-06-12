#ifndef GAMEPAD_H
#define GAMEPAD_H
#include <cstdint>
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

	enum class Axis : int16_t
	{
		invalid = -1,
		lStick,
		rStick,
		triggers,
	};
	// a bit easier to read & work with than constantly having to static cast
	static constexpr int maskBits{ static_cast<int>( Gamepad::Button::count ) };

	Gamepad();
	~Gamepad();

	void AddGamepad();
	void RemoveGamepad();

	std::pair<int16_t, int16_t> PollAxis( Axis axis );

	void UpdateGamepad();
	std::bitset<maskBits> GetMask();
	std::bitset<maskBits> GetPreviousMask();

	static int RemapButtonToKey( Button in );
	static std::bitset<maskBits> GetMaskFromButtonID( Gamepad::Button in );

private:
	class GamepadImpl;
	std::unique_ptr<GamepadImpl> m_pImpl{};
};
} // namespace dae
#endif
