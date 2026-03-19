#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H
#include "Engine/Helpers/Subscription.h"
#include "Engine/Patterns/Component.h"
#include "Engine/Patterns/Observer.h"
#include <cstdint>

namespace dae
{
class ScoreComponent : public Component
{
public:
	ScoreComponent( GameObject* pParent, uint32_t startingScore = 0 );
	virtual ~ScoreComponent() = default;

	virtual void Update() override
	{
	}

	void Accumulate( uint32_t increase );
	uint32_t GetScore() const;

	void RegisterObserver( Observer<ScoreComponent>* pObserver );
	void RemoveObserver( Observer<ScoreComponent>* pObserver );

private:
	uint32_t m_Score{};
	Messenger<ScoreComponent> m_Subject;
	Subscription m_Subscription;

	void HandleEvent( Event& event );
};
} // namespace dae
#endif
