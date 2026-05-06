#ifndef HURTBOXCOMPONENT_H
#define HURTBOXCOMPONENT_H
#include <Patterns.h>
#include <Hitboxes.h>

namespace dae
{
class HurtboxComponent : public Component
{
public:
	HurtboxComponent( GameObject* pParent,
					  const glm::vec4& baseRect,
					  size_t targetHash,
					  const std::function<void( GameObject*, Hitbox* )> callback );

	virtual void Update() override;

private:
	Hurtbox m_Hurtbox{};
	glm::vec4 m_BaseRect{};
};
}; // namespace dae
#endif
