#ifndef GAMEPAD_H
#define GAMEPAD_H
#include <cstdint>
#include <memory>

namespace dae
{
class Gamepad final
{
public:
	enum class Button : uint16_t
	{
		invalid = 0,
		up = 1 << 0,
		down = 1 << 1,
		left = 1 << 2,
		right = 1 << 3,
		start = 1 << 4,
		select = 1 << 5,
		l3 = 1 << 6,
		r3 = 1 << 7,
		l1 = 1 << 8,
		r1 = 1 << 9,
		// Platform agnostic face buttons
		south = 1 << 10, // (XBox A; Nintendo B; PS Cross)
		east = 1 << 11,	 // (XBox B; Nintendo A; PS Circle)
		west = 1 << 12,	 // (XBox X; Nintendo Y; PS Square)
		north = 1 << 13, // (XBox Y; Nintendo X; PS Triangle)
		count = 14,
	};

	Gamepad();
	~Gamepad();

	void UpdateGamepad();

private:
	class GamepadImpl;
	std::unique_ptr<GamepadImpl> m_pImpl{};
};
} // namespace dae
#endif
