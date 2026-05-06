#ifndef TEXTALLIGNMENTCOMPONENT_H
#define TEXTALLIGNMENTCOMPONENT_H
#include <Patterns.h>
#include <glm/glm.hpp>
#include "Game/Components/TextComponent.h"

namespace dae
{
class TextAllignmentComponent : public Component
{
public:
	enum class Allignment
	{
		topLeft,
		topRight,
		bottomLeft,
		bottomRight,
	};

	TextAllignmentComponent( GameObject* pParent, glm::vec2 origin, Allignment allignment );
	virtual ~TextAllignmentComponent() = default;

	virtual void Update() override;

private:
	ReferencePtr<TextComponent> m_TextRef{};
	glm::vec2 m_Origin{};
	Allignment m_Allignment{};
};
} // namespace dae
#endif
