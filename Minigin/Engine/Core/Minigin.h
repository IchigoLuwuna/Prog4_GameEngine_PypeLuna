#ifndef MINIGIN_H
#define MINIGIN_H
#include <functional>
#include <filesystem>
#include "Engine/Core/EventManager.h"

namespace dae
{
class Minigin final
{
public:
	explicit Minigin( const std::filesystem::path& dataPath );
	~Minigin();
	void Run( const std::function<void()>& load );
	void RunOneFrame();

	Minigin( const Minigin& other ) = delete;
	Minigin( Minigin&& other ) = delete;
	Minigin& operator=( const Minigin& other ) = delete;
	Minigin& operator=( Minigin&& other ) = delete;

	static EventManager eventManager;

private:
	bool m_Quit{};
};
} // namespace dae
#endif
