#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H
#include <Patterns.h>
#include <Helpers.h>
#include <cstdint>

namespace dae
{
class ScoreComponent : public Component
{
public:
	ScoreComponent( GameObject* pParent,
					const std::vector<std::pair<size_t, uint32_t>>& scoreGainOnEvents,
					uint32_t startingScore = 0 );
	ScoreComponent( GameObject* pParent,
					std::vector<std::pair<size_t, uint32_t>>&& scoreGainOnEvents,
					uint32_t startingScore = 0 );
	virtual ~ScoreComponent() = default;

	virtual void Update() override
	{
	}

	void Accumulate( uint32_t increase );
	uint32_t GetScore() const;

	template <typename T>
	void RegisterObserver( ReferencePtr<T> pObserver )
	{
		m_Messenger.RegisterObserver( pObserver );
	}
	void RemoveObserver( Observer<ScoreComponent>* pObserver );

private:
	Messenger<ScoreComponent> m_Messenger;
	Subscription m_Subscription;
	std::vector<std::pair<size_t, uint32_t>> m_ScoreGainOnEvents{};
	uint32_t m_Score{};

	void HandleEvent( Event& event );
};
} // namespace dae
#endif
