#ifndef DEATHCALLBACKCOMPONENT_H
#define DEATHCALLBACKCOMPONENT_H
#include <functional>
#include <Patterns.h>

namespace dae
{
class DeathCallbackComponent : public Component
{
public:
	DeathCallbackComponent( GameObject* pParent, const std::function<void()>& callback )
		: Component( pParent )
		, m_Callback( callback )
	{
	}
	~DeathCallbackComponent()
	{
		m_Callback();
	}

	virtual void Update() override
	{
	}

private:
	std::function<void()> m_Callback{};
};
} // namespace dae
#endif
