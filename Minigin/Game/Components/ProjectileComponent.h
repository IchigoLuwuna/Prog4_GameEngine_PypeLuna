#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H
#include <Patterns.h>
#include <glm/glm.hpp>

namespace dae
{
class ProjectileComponent : public Component
{
public:
	ProjectileComponent( GameObject* pParent, const glm::vec2& velocity, float lifetime );
	virtual ~ProjectileComponent() = default;

	virtual void Update() override;

private:
	glm::vec2 m_Velocity{};
	float m_Lifetime{};
	Messenger<ProjectileComponent> m_Messenger;
};
} // namespace dae
#endif
