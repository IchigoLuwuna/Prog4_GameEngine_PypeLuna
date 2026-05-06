#ifndef HURTBOX_H
#define HURTBOX_H
#include <glm/glm.hpp>

namespace dae
{
class GameObject;
class Hitbox;

class Hurtbox
{
public:
	Hurtbox() = default;
	Hurtbox( GameObject* pParent,
			 const glm::vec4& rect,
			 size_t targetHash,
			 const std::function<void( GameObject*, Hitbox* )> callback );
	~Hurtbox();

	GameObject* GetParent() const;

	void OnCollision( Hitbox* pOther );

	// Trivial get/set
	glm::vec4 m_Rectangle{};

private:
	GameObject* m_pParent{};
	size_t m_TargetHash{};
	std::function<void( GameObject*, Hitbox* )> m_Callback{};
};
} // namespace dae
#endif
