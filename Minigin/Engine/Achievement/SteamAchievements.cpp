#if USE_STEAMWORKS
#	include "SteamAchievements.h"
#	include <isteamuserstats.h>

dae::steam::SteamAchievements::SteamAchievements( const std::vector<Achievement>& achievements )
	: m_AppId( SteamUtils()->GetAppID() )
	, m_Achievements( achievements )
	, m_Initialized( IsSteamInitialized() )
	, m_CallbackUserStatsStored( this, &SteamAchievements::OnUserStatsStored )
	, m_CallbackAchievementStored( this, &SteamAchievements::OnAchievementStored )
{
}

dae::steam::SteamAchievements::SteamAchievements( std::vector<Achievement>&& achievements )
	: m_AppId( SteamUtils()->GetAppID() )
	, m_Achievements( std::move( achievements ) )
	, m_Initialized( IsSteamInitialized() )
	, m_CallbackUserStatsStored( this, &SteamAchievements::OnUserStatsStored )
	, m_CallbackAchievementStored( this, &SteamAchievements::OnAchievementStored )
{
}

dae::steam::SteamAchievements& dae::steam::SteamAchievements::operator=( const std::vector<Achievement>& achievements )
{
	m_Initialized = IsSteamInitialized();
	m_Achievements = achievements;

	return *this;
}

dae::steam::SteamAchievements& dae::steam::SteamAchievements::operator=( std::vector<Achievement>&& achievements )
{
	m_Initialized = IsSteamInitialized();
	m_Achievements = std::move( achievements );

	return *this;
}

void dae::steam::SteamAchievements::SetAchievement( const char* id )
{
	if ( !m_Initialized )
	{
		return;
	}

	SteamUserStats()->SetAchievement( id );
	SteamUserStats()->StoreStats();
}

bool dae::steam::SteamAchievements::IsSteamInitialized()
{
	if ( !SteamUserStats() || !SteamUser() )
	{
		return false;
	}

	if ( !SteamUser()->BLoggedOn() )
	{
		return false;
	}

	return true;
}

void dae::steam::SteamAchievements::OnUserStatsStored( UserStatsStored_t* pCallback )
{
	if ( pCallback->m_nGameID != m_AppId )
	{
		return;
	}
}

void dae::steam::SteamAchievements::OnAchievementStored( UserAchievementStored_t* pCallback )
{
	if ( pCallback->m_nGameID != m_AppId )
	{
		return;
	}
}
#endif
