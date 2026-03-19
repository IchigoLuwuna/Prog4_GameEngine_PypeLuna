#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H
#include <functional>

namespace dae
{
struct Event;
class Subscription final
{
public:
	Subscription( void* id, std::function<void( Event& event )> handler );
	~Subscription();

	Subscription( const Subscription& ) = delete;
	Subscription( Subscription&& other );
	Subscription& operator=( const Subscription& ) = delete;
	Subscription& operator=( Subscription&& other );

private:
	void* m_Id{};
};
} // namespace dae
#endif
