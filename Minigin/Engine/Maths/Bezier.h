#ifndef BEZIER_H
#define BEZIER_H
#include <array>
#include <glm/glm.hpp>
#include <cmath>

namespace dae::math
{
struct Bezier3
{
	glm::vec2 Lerp( float lerp )
	{
		auto a{ controlPoints[0] };
		auto b{ controlPoints[1] };
		auto c{ controlPoints[2] };

		glm::vec2 ab{
			std::lerp( a.x, b.x, lerp ),
			std::lerp( a.y, b.y, lerp ),
		};
		glm::vec2 bc{
			std::lerp( b.x, c.x, lerp ),
			std::lerp( b.y, c.y, lerp ),
		};

		glm::vec2 abc{
			std::lerp( ab.x, bc.x, lerp ),
			std::lerp( ab.y, bc.y, lerp ),
		};

		return abc;
	}

	std::array<glm::vec2, 3> controlPoints{};
};

struct Bezier4
{
	glm::vec2 Lerp( float lerp )
	{
		auto a{ controlPoints[0] };
		auto b{ controlPoints[1] };
		auto c{ controlPoints[2] };
		auto d{ controlPoints[3] };

		glm::vec2 ab{
			std::lerp( a.x, b.x, lerp ),
			std::lerp( a.y, b.y, lerp ),
		};
		glm::vec2 bc{
			std::lerp( b.x, c.x, lerp ),
			std::lerp( b.y, c.y, lerp ),
		};
		glm::vec2 cd{
			std::lerp( c.x, d.x, lerp ),
			std::lerp( c.y, d.y, lerp ),
		};

		glm::vec2 abc{
			std::lerp( ab.x, bc.x, lerp ),
			std::lerp( ab.y, bc.y, lerp ),
		};
		glm::vec2 bcd{
			std::lerp( bc.x, cd.x, lerp ),
			std::lerp( bc.y, cd.y, lerp ),
		};

		glm::vec2 abcd{
			std::lerp( abc.x, bcd.x, lerp ),
			std::lerp( abc.y, bcd.y, lerp ),
		};

		return abcd;
	}

	std::array<glm::vec2, 4> controlPoints{};
};
} // namespace dae::math
#endif
