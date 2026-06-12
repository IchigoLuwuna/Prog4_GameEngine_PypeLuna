#ifndef MINIGIN_H
#define MINIGIN_H
#include <functional>
#include <filesystem>
#include <glm/glm.hpp>
#include "EventManager.h"
#include "Achievement/SteamAchievements.h"

namespace dae
{
class Minigin final
{
public:
	struct WindowSettings
	{
		const char* name{};
		glm::vec2 resolution{};
		glm::vec2 internalResolution{};
	};

	explicit Minigin( const WindowSettings& settings, const std::filesystem::path& dataPath );
	~Minigin();
	void Run( const std::function<void()>& load );
	void RunOneFrame();

	Minigin( const Minigin& other ) = delete;
	Minigin( Minigin&& other ) = delete;
	Minigin& operator=( const Minigin& other ) = delete;
	Minigin& operator=( Minigin&& other ) = delete;

#if USE_STEAMWORKS
	static std::unique_ptr<steam::SteamAchievements> steamAchievements;
#endif

	static EventManager eventManager;

private:
	bool m_Quit{};
};
} // namespace dae
#endif
