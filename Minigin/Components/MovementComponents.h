#ifndef MOVEMENTCOMPONENTS_H
#define MOVEMENTCOMPONENTS_H
#include <glm/glm.hpp>
#include "Engine/Patterns/Component.h"
// This header contains a collection of different generic and automatic movement behaviours
namespace dae
{
class SlideMovementComponent : public Component
{
public:
	SlideMovementComponent( GameObject* pParent, const glm::vec3& movement );
	virtual ~SlideMovementComponent() = default;

	virtual void Update() override;

private:
	glm::vec3 m_Movement{};
};

class OrbitMovementComponent : public Component
{
public:
	OrbitMovementComponent( GameObject* pParent, float distance, float speed, float startingAngle = 0.f );
	virtual ~OrbitMovementComponent() = default;

	virtual void Update() override;

private:
	float m_Distance{};
	float m_Speed{};
	float m_Angle{};
};
} // namespace dae

#endif
