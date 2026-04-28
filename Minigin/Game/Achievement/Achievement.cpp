#include "Achievement.h"
#include <cstdint>
#include "Engine/Core/EventManager.h"
#include "Engine/Helpers/SdbmHash.h"
#include "Engine/Core/Minigin.h"

void CheckWinOneGame( void* pData );

void dae::achievements::HandleEvent( Event& event )
{
	switch ( event.eventHash )
	{
	case "e_ScoreChanged"_hash: {
		CheckWinOneGame( event.pData );
	}
	default: {
		return;
	}
	}
}

void CheckWinOneGame( void* pData )
{
	auto pScore{ reinterpret_cast<uint32_t*>( pData ) };
	if ( *pScore >= 500 )
	{
#if USE_STEAMWORKS
		dae::Minigin::steamAchievements->SetAchievement( "ACH_WIN_ONE_GAME" );
#endif
	}
}
