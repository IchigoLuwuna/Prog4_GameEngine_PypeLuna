#ifndef HEALTHDISPLAYCOMPONENT_H
#define HEALTHDISPLAYCOMPONENT_H
#include "HealthComponent.h"
#include <Patterns.h>
#include <Helpers.h>

namespace dae
{
class TextComponent;
class HealthDisplayComponent : public Component, public Observer<HealthComponent>
{
public:
	HealthDisplayComponent( GameObject* pParent ); // Initialization needs to be two-stage
	virtual ~HealthDisplayComponent() = default;

	HealthDisplayComponent& SetSubjectHealth( const ReferencePtr<HealthComponent>& subject );

	virtual void Update() override
	{
	}

	virtual void Notify( size_t eventHash, HealthComponent* pSubject ) override;

private:
	ReferencePtr<TextComponent> m_pText{};

	void UpdateText( HealthComponent* pHealth );
	void HandleEvent( Event& event );
};
} // namespace dae
#endif
