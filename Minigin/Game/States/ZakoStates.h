#ifndef ZAKOSTATES_H
#define ZAKOSTATES_H
#include <States.h>

namespace dae
{
// The bee guys
class GameObject;
class ZakoState : public State
{
public:
	ZakoState( void* pParent )
		: State( pParent )
	{
	}
	virtual ~ZakoState() = default;
	virtual void Update( GameObject* pParent ) = 0;
};

class ZakoIdlingState final : public ZakoState
{
public:
	ZakoIdlingState( void* pParent );
	virtual void Update( GameObject* pObject ) override;

private:
	float m_RemainingStateTime{};

	constexpr static float m_MaxStateTime{ 10.f };
};

class ZakoDivingState final : public ZakoState
{
public:
	ZakoDivingState( void* pParent );
	virtual void Update( GameObject* pObject ) override;
};

class ZakoReturningState final : public ZakoState
{
public:
	ZakoReturningState( void* pParent );
	virtual void Update( GameObject* pObject ) override;
};
} // namespace dae
#endif
