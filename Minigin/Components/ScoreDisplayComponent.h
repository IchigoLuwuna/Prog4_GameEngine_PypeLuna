#ifndef SCOREDISPLAYCOMPONENT_H
#define SCOREDISPLAYCOMPONENT_H
#include "Engine/Helpers/Subscription.h"
#include "Engine/Memory/ReferencePtr.h"
#include "Engine/Patterns/Component.h"
#include "Engine/Patterns/Observer.h"

namespace dae
{
class ScoreComponent;
class TextComponent;
class ScoreDisplayComponent : public Component, Observer<ScoreComponent>
{
public:
	ScoreDisplayComponent( GameObject* pParent, const ReferencePtr<ScoreComponent>& pScore );
	virtual ~ScoreDisplayComponent();

	virtual void Update() override
	{
	}

	virtual void Notify( size_t eventHash, ScoreComponent* pSubject ) override;

private:
	ReferencePtr<TextComponent> m_pText{};
	ReferencePtr<ScoreComponent> m_pSubject{};

	void UpdateText( ScoreComponent* pScore );
	void HandleEvent( Event& event );
};
} // namespace dae
#endif
