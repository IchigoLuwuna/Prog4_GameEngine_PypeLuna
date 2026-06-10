#ifndef HIVEMIND_H
#define HIVEMIND_H
#include <bitset>
#include <Patterns.h>
#include <glm/glm.hpp>

// A lot of static
namespace dae
{
class HiveMind
{
public:
	static constexpr uint32_t m_FormationRows{ 5 };
	static constexpr uint32_t m_FormationColumns{ 10 };

	enum class HiveMindType
	{
		zako,
		goei,
		galaga,
	};

	HiveMind() = default;
	virtual ~HiveMind() = default;

	GameObject* GetPlayer();
	uint64_t GetFormationSlot( HiveMindType type );
	glm::vec2 GetFormationPosition( uint64_t slot ) const;

	void ClearFormationSlot( uint64_t slot );

private:
	static constexpr glm::vec2 m_FormationStartPos{ 24, 37 };
	static constexpr glm::vec2 m_FormationSlotSize{ 16.f, 16.f };

	static std::array<std::bitset<m_FormationColumns>, m_FormationRows> m_FormationFilled;
	static glm::vec2 m_FormationOffset;
	static GameObject* m_pPlayer;

	uint64_t GetZakoSlot() const;
	uint64_t GetGoeiSlot() const;
	uint64_t GetGalagaSlot() const;

	constexpr uint64_t Combine( uint32_t first, uint32_t second ) const;
	constexpr std::pair<uint32_t, uint32_t> Split( uint64_t combined ) const;
};
} // namespace dae
#endif
