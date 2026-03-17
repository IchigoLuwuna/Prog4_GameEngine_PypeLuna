#ifndef HEALTHDISPLAYCOMPONENT_H
#define HEALTHDISPLAYCOMPONENT_H
#include "Components/HealthComponent.h"
#include "Engine/Patterns/Component.h"
#include "Engine/Patterns/Observer.h"

namespace dae
{
class TextComponent;
class HealthDisplayComponent : public Component, public Observer<HealthComponent>
{
public:
	HealthDisplayComponent( GameObject* pParent, HealthComponent* pHealth );
	virtual ~HealthDisplayComponent();

	virtual void Update() override
	{
	}

	virtual void Notify( size_t eventHash, HealthComponent* pSubject ) override;
	void UpdateText( HealthComponent* pHealth );

private:
	TextComponent* m_pText{};
	HealthComponent* m_pSubject{};
};
} // namespace dae
#endif
