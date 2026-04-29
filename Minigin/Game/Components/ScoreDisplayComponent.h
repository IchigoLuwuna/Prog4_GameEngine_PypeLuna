#ifndef SCOREDISPLAYCOMPONENT_H
#define SCOREDISPLAYCOMPONENT_H
#include <Patterns.h>
#include <Core.h>

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
