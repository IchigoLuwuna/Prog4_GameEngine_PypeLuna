#ifndef LISTENER_H
#define LISTENER_H
#include <functional>

namespace dae
{
struct Event;
class Listener final
{
public:
	Listener( void* id, std::function<void( Event& event )> handler );
	~Listener();

	Listener( const Listener& ) = delete;
	Listener( Listener&& other );
	Listener& operator=( const Listener& ) = delete;
	Listener& operator=( Listener&& other );

private:
	void* m_Id{};
};
} // namespace dae
#endif
