#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H
#include <Patterns.h>
#include <Hitboxes.h>
#include <glm/glm.hpp>

namespace dae
{
class ProjectileComponent : public Component
{
public:
	ProjectileComponent( GameObject* pParent, const glm::vec2& velocity, float lifetime );
	virtual ~ProjectileComponent();

	virtual void Update() override;

	template <typename T>
	void RegisterObserver( ReferencePtr<T> pObserver )
	{
		m_Messenger.RegisterObserver( pObserver );
	}
	void RemoveObserver( Observer<ProjectileComponent>* pObserver );

	void Hit();

private:
	glm::vec2 m_Velocity{};
	float m_Lifetime{};
	Messenger<ProjectileComponent> m_Messenger;
};
} // namespace dae
#endif
