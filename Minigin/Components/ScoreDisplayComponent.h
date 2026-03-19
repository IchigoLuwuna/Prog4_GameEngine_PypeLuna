#ifndef SCOREDISPLAYCOMPONENT_H
#define SCOREDISPLAYCOMPONENT_H
#include "Engine/Helpers/Subscription.h"
#include "Engine/Patterns/Component.h"
#include "Engine/Patterns/Observer.h"

namespace dae
{
class ScoreComponent;
class TextComponent;
class ScoreDisplayComponent : public Component, Observer<ScoreComponent>
{
public:
	ScoreDisplayComponent( GameObject* pParent, ScoreComponent* pScore );
	virtual ~ScoreDisplayComponent();

	virtual void Update() override
	{
	}

	virtual void Notify( size_t eventHash, ScoreComponent* pSubject ) override;

private:
	TextComponent* m_pText{};
	ScoreComponent* m_pSubject{};
	Subscription m_Listener;

	void UpdateText( ScoreComponent* pScore );
	void HandleEvent( Event& event );
};
} // namespace dae
#endif
