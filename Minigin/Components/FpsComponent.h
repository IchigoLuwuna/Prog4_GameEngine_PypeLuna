#ifndef FPSCOMPONENT_H
#define FPSCOMPONENT_H
#include "Engine/Patterns/Component.h"

namespace dae
{
class TextComponent;
class FpsComponent : public Component
{
public:
	FpsComponent( GameObject* pParent )
		: Component( pParent )
	{
	}
	virtual ~FpsComponent() = default;

	virtual void Update() override;

private:
	float m_Fps{};
	float m_LastUpdate{};
	TextComponent* m_pTextComponent{};
};
} // namespace dae
#endif
