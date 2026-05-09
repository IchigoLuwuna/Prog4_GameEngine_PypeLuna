#ifndef SCOREDISPLAYCOMPONENT_H
#define SCOREDISPLAYCOMPONENT_H
#include <Patterns.h>
#include <Core.h>

namespace dae
{
class ScoreComponent;
class TextComponent;
class ScoreDisplayComponent : public Component, public Observer
{
public:
	ScoreDisplayComponent( GameObject* pParent );
	virtual ~ScoreDisplayComponent() = default;

	ScoreDisplayComponent& SetSubjectScore( const ReferencePtr<ScoreComponent>& subject );

	virtual void Update() override
	{
	}

	virtual void Notify( size_t eventHash, void* pSubject ) override;

private:
	ReferencePtr<TextComponent> m_pText{};

	void UpdateText( ScoreComponent* pScore );
	void HandleEvent( Event& event );
};
} // namespace dae
#endif
