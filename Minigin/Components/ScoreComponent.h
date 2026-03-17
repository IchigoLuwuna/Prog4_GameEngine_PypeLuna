#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H
#include "Engine/Patterns/Component.h"
#include "Engine/Patterns/Observer.h"
#include <cstdint>

namespace dae
{
class ScoreComponent : public Component
{
public:
	ScoreComponent( GameObject* pParent, uint32_t startingScore = 0 )
		: Component( pParent )
		, m_Score( startingScore )
		, m_Subject( this )
	{
	}
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
	Subject<ScoreComponent> m_Subject;
};
} // namespace dae
#endif
