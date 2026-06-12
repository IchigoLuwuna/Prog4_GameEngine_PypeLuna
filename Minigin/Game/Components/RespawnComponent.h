#ifndef RESPAWNCOMPONENT_H
#define RESPAWNCOMPONENT_H
#include <functional>
#include <memory>
#include <Patterns.h>
#include <glm/glm.hpp>

namespace dae
{
class RespawnComponent : public Component
{
public:
	RespawnComponent( GameObject* pParent,
					  std::function<std::unique_ptr<GameObject>()> respawnFunction,
					  glm::vec2 respawnPoint,
					  float timer );

	virtual void Update() override;

private:
	std::function<std::unique_ptr<GameObject>()> m_RespawnFunction{};
	glm::vec2 m_RespawnPoint{};
	float m_RespawnTimer{};
};
} // namespace dae
#endif
