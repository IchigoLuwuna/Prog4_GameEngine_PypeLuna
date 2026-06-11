#include "HiveMind.h"
#include "Game/Context.h"
#include "Patterns/GameObject.h"
#include <Scene.h>
#include <Components.h>
#include <Helpers.h>

std::array<std::bitset<dae::HiveMind::m_FormationColumns>, dae::HiveMind::m_FormationRows>
	dae::HiveMind::m_FormationFilled{};
dae::GameObject* dae::HiveMind::m_pPlayer{};

dae::GameObject* dae::HiveMind::GetPlayer()
{
	// If player ptr is set -> early return (optimal)
	// If player ptr is clear -> search for it every 1.f seconds
	// If player ptr is clear, but 1.f seconds have not elapsed -> return nullptr
	// If player ptr is clear, but no player is found -> return nullptr and wait another 1.f secs
	// This alleviates slowdown from repeated searches in the hot code path

	static Validator m_PlayerValidator{};

	if ( m_PlayerValidator.Validate() )
	{
		return m_pPlayer;
	}
	constexpr float interval{ 1.f };
	static float lastPlayerRequest{ dae::Timer::GetInstance().GetTotalElapsed() - interval };

	if ( dae::Timer::GetInstance().GetTotalElapsed() < lastPlayerRequest + interval )
	{
		return nullptr;
	}

#ifndef NDEBUG
	std::cout << "Player ptr not yet set, finding player\n";
#endif
	lastPlayerRequest = dae::Timer::GetInstance().GetTotalElapsed();

	m_pPlayer = dae::SceneManager::GetInstance().GetScene( gameIdx ).GetByTag( "player"_hash );
	if ( !m_pPlayer )
	{
		return m_pPlayer;
	}

	m_PlayerValidator = Validator{ m_pPlayer->GetComponent<dae::TransformComponent>().GetControlBlock() };
	return m_pPlayer;
}

uint64_t dae::HiveMind::GetFormationSlot( HiveMindType type )
{
	switch ( type )
	{
	case HiveMindType::zako: {
		auto openSlot{ GetZakoSlot() };
		if ( openSlot == -1u )
		{
			return openSlot;
		}
		auto split{ Split( openSlot ) };
		m_FormationFilled[split.first].set( split.second );
		return openSlot;
		break;
	}
	case HiveMindType::goei: {
		auto openSlot{ GetGoeiSlot() };
		if ( openSlot == -1u )
		{
			return openSlot;
		}
		auto split{ Split( openSlot ) };
		m_FormationFilled[split.first].set( split.second );
		return openSlot;
		break;
	}
	case HiveMindType::boss: {
		auto openSlot{ GetGalagaSlot() };
		if ( openSlot == -1u )
		{
			return openSlot;
		}
		auto split{ Split( openSlot ) };
		m_FormationFilled[split.first].set( split.second );
		return openSlot;
		break;
	}
	default: {
		assert( false && "Invalid type" );
	}
	}

	return -1u;
}

glm::vec2 dae::HiveMind::GetFormationPosition( uint64_t slot ) const
{
	auto split{ Split( slot ) };
	auto row{ split.first };
	auto column{ split.second };

	glm::vec2 offset{
		m_FormationSlotSize.x * column,
		m_FormationSlotSize.y * row,
	};

	return m_FormationStartPos + offset;
}

void dae::HiveMind::ClearFormationSlot( uint64_t slot )
{
	if ( slot == -1u )
	{
		return;
	}

	auto split{ Split( slot ) };
	m_FormationFilled[split.first].set( split.second, false );
}

bool dae::HiveMind::IsFormationClear()
{
	bool any{};
	for ( auto& row : m_FormationFilled )
	{
		any |= row.any();
	}

	return !any;
}

uint64_t dae::HiveMind::GetZakoSlot() const
{
	const uint32_t topRow{ 3 };
	const uint32_t bottomRow{ 4 };
	int leftIdx{ m_FormationColumns / 2 };
	uint32_t rightIdx{ m_FormationColumns / 2 };
	while ( leftIdx > 0 || rightIdx < m_FormationColumns )
	{
		if ( !m_FormationFilled[topRow].test( rightIdx ) )
		{
			return Combine( topRow, rightIdx );
		}
		if ( !m_FormationFilled[bottomRow].test( rightIdx ) )
		{
			return Combine( bottomRow, rightIdx );
		}
		if ( !m_FormationFilled[topRow].test( leftIdx ) )
		{
			return Combine( topRow, leftIdx );
		}
		if ( !m_FormationFilled[bottomRow].test( leftIdx ) )
		{
			return Combine( bottomRow, leftIdx );
		}

		if ( leftIdx >= 0 )
		{
			--leftIdx;
		}
		if ( rightIdx < m_FormationColumns )
		{
			++rightIdx;
		}
	}

	return -1u;
}

uint64_t dae::HiveMind::GetGoeiSlot() const
{
	const uint32_t topRow{ 1 };
	const uint32_t bottomRow{ 2 };
	int leftIdx{ m_FormationColumns / 2 };
	uint32_t rightIdx{ m_FormationColumns / 2 };
	while ( leftIdx > 0 || rightIdx < m_FormationColumns )
	{
		if ( !m_FormationFilled[topRow].test( rightIdx ) )
		{
			return Combine( topRow, rightIdx );
		}
		if ( !m_FormationFilled[bottomRow].test( rightIdx ) )
		{
			return Combine( bottomRow, rightIdx );
		}
		if ( !m_FormationFilled[topRow].test( leftIdx ) )
		{
			return Combine( topRow, leftIdx );
		}
		if ( !m_FormationFilled[bottomRow].test( leftIdx ) )
		{
			return Combine( bottomRow, leftIdx );
		}

		if ( leftIdx >= 0 )
		{
			--leftIdx;
		}
		if ( rightIdx < m_FormationColumns )
		{
			++rightIdx;
		}
	}

	return -1u;
}

uint64_t dae::HiveMind::GetGalagaSlot() const
{
	const uint32_t bossRow{ 0 };
	int leftIdx{ m_FormationColumns / 2 };
	uint32_t rightIdx{ m_FormationColumns / 2 };
	while ( leftIdx > 0 || rightIdx < m_FormationColumns )
	{
		if ( !m_FormationFilled[bossRow].test( rightIdx ) )
		{
			return Combine( bossRow, rightIdx );
		}
		if ( !m_FormationFilled[bossRow].test( leftIdx ) )
		{
			return Combine( bossRow, leftIdx );
		}

		if ( leftIdx >= 0 )
		{
			--leftIdx;
		}
		if ( rightIdx < m_FormationColumns )
		{
			++rightIdx;
		}
	}

	return -1u;
}

constexpr uint64_t dae::HiveMind::Combine( uint32_t first, uint32_t second ) const
{
	uint64_t result{ second };
	result <<= 32;
	result |= first;
	return result;
}

constexpr std::pair<uint32_t, uint32_t> dae::HiveMind::Split( uint64_t combined ) const
{
	uint32_t first{ static_cast<uint32_t>( combined & 0xFF'FF'FF'FF ) };
	uint32_t second{ static_cast<uint32_t>( ( combined >> 32 ) & 0xFF'FF'FF'FF ) };

	return { first, second };
}
