#ifndef TRACTORCOMPONENT_H
#define TRACTORCOMPONENT_H
#include <Patterns.h>
#include <Components.h>

namespace dae
{
class TractorComponent : public Component
{
public:
	TractorComponent( GameObject* pParent );
	~TractorComponent();

	virtual void Update() override;

private:
	ReferencePtr<HitboxComponent> m_HitboxRef{};
	float m_TractoringTime{};
	int m_CurrentPhase{};
};
} // namespace dae
#endif
