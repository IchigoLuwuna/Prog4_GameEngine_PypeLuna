#ifndef SOUNDCOMPONENT_H
#define SOUNDCOMPONENT_H
#include "Engine/Memory/ReferencePtr.h"
#include "Engine/Patterns/Component.h"
#include "Engine/Helpers/Subscription.h"
#include "Engine/Patterns/GameObject.h"
#include <string>

// This component can hook into events

namespace dae
{
class ReactiveSoundComponent : public Component
{
public:
	ReactiveSoundComponent( GameObject* pParent );
	~ReactiveSoundComponent();
	struct ReactiveSoundInfo
	{
		const size_t eventHash{}; // What to react to with a sound
		const void*
			pSubject{}; // Limit scope to a single subject (important for protecting your ears and being correct)
		const std::string path{}; // Sound to play
	};

	virtual void Update() override { };
	ReactiveSoundComponent& AddSound( const ReactiveSoundInfo& info );
	ReactiveSoundComponent& AddSound( ReactiveSoundInfo&& info );

	template <typename T>
		requires requires( T subject ) { subject.RegisterObserver( nullptr ); }
	ReactiveSoundComponent& AddSubject( ReferencePtr<T> pSubject )
	{
		pSubject->RegisterObserver( GetParent()->GetComponent<ReactiveSoundComponent>() );
		return *this;
	}

private:
	std::vector<ReactiveSoundInfo> m_ReactiveSounds{};
	Subscription m_Subscription;

	void HandleEvent( Event& event );
};
} // namespace dae
#endif
