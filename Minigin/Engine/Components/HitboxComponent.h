#ifndef HITBOXCOMPONENT_H
#define HITBOXCOMPONENT_H
#include <Patterns.h>
#include <Hitboxes.h>

namespace dae
{
class HitboxComponent : public Component
{
public:
	HitboxComponent( GameObject* pParent,
					 const glm::vec4& baseRect,
					 const std::vector<size_t>& targetHashes,
					 const std::function<void( GameObject*, Hurtbox* )> callback );

	virtual void Update() override;

private:
	Hitbox m_Hitbox{};
	glm::vec4 m_BaseRect{};
};
}; // namespace dae
#endif
