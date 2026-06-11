#ifndef SPRITEALLIGNMENTCOMPONENT_H
#define SPRITEALLIGNMENTCOMPONENT_H
#include <Patterns.h>
#include <glm/glm.hpp>
#include "Game/Components/SpriteSheetComponent.h"

namespace dae
{
class SpriteAllignmentComponent : public Component
{
public:
	enum class Allignment
	{
		center,
		topLeft,
		topRight,
		bottomLeft,
		bottomRight,
	};

	SpriteAllignmentComponent( GameObject* pParent, glm::vec2 origin, Allignment allignment );
	virtual ~SpriteAllignmentComponent() = default;

	virtual void Update() override;

private:
	ReferencePtr<SpriteSheetComponent> m_SpritesheetRef{};
	glm::vec2 m_Origin{};
	Allignment m_Allignment{};
};
} // namespace dae
#endif
