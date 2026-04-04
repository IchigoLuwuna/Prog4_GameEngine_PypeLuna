#ifndef HEALTHDISPLAYCOMPONENT_H
#define HEALTHDISPLAYCOMPONENT_H
#include "Components/HealthComponent.h"
#include "Engine/Helpers/Subscription.h"
#include "Engine/Memory/ReferencePtr.h"
#include "Engine/Patterns/Component.h"
#include "Engine/Patterns/Observer.h"

namespace dae
{
class TextComponent;
class HealthDisplayComponent : public Component, public Observer<HealthComponent>
{
public:
	HealthDisplayComponent( GameObject* pParent, const ReferencePtr<HealthComponent>& pHealth );
	virtual ~HealthDisplayComponent();

	virtual void Update() override
	{
	}

	virtual void Notify( size_t eventHash, HealthComponent* pSubject ) override;

private:
	ReferencePtr<TextComponent> m_pText{};
	ReferencePtr<HealthComponent> m_pSubject{};

	void UpdateText( HealthComponent* pHealth );
	void HandleEvent( Event& event );
};
} // namespace dae
#endif
