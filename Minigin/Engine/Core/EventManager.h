#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include "Engine/Patterns/Singleton.h"
#include <functional>
#include <vector>
#include <cstddef>

namespace dae
{
struct Event
{
	size_t EventHash{};
	void* pData{};
};

class EventManager final : public Singleton<EventManager>
{
public:
	void AttachListener( void* pListener, std::function<void( Event& event )> handler );
	void DetachListener( void* pListener );
	void ProcessEvents();
	void SendEvent( const Event& event );

private:
	friend class Singleton<EventManager>;
	EventManager() = default;

	std::array<Event, 64> m_Events{};
	size_t m_EventsBack{};
	std::vector<std::pair<void*, std::function<void( Event& event )>>> m_Listeners{};
};
} // namespace dae
#endif
