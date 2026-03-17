#if USE_STEAMWORKS
#	ifndef STEAMACHIEVEMENTS_H
#		define STEAMACHIEVEMENTS_H
#		include <vector>
#		include <isteamuserstats.h>
#		include <steam_api.h>

namespace dae
{
namespace steam
{
#		define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }
struct Achievement
{
	int id{};
	const char* apiName{};
	char name[128];
	char description[256];
	bool achieved{};
	int iconId{};
};

class SteamAchievements
{
public:
	explicit SteamAchievements( const std::vector<Achievement>& achievements );
	explicit SteamAchievements( std::vector<Achievement>&& achievements );

	SteamAchievements& operator=( const std::vector<Achievement>& achievements );
	SteamAchievements& operator=( std::vector<Achievement>&& achievements );

	void SetAchievement( const char* id );

private:
	uint64_t m_AppId{};
	std::vector<Achievement> m_Achievements{};
	bool m_Initialized{};

	STEAM_CALLBACK( SteamAchievements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored );
	STEAM_CALLBACK( SteamAchievements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored );

	bool IsSteamInitialized();
};
} // namespace steam
} // namespace dae
#	endif
#endif
