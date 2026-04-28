#ifndef FPSCOMPONENT_H
#define FPSCOMPONENT_H
#include "Engine/Memory/ReferencePtr.h"
#include "Engine/Patterns/Component.h"

namespace dae
{
class TextComponent;
class FpsComponent : public Component
{
public:
	FpsComponent( GameObject* pParent );
	virtual ~FpsComponent() = default;

	virtual void Update() override;
	void UpdateText();

private:
	float m_Fps{};
	float m_LastUpdate{};
	ReferencePtr<TextComponent> m_pTextComponent{};
};
} // namespace dae
#endif
