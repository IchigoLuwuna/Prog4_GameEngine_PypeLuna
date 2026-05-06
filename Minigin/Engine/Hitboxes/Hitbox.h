#ifndef HITBOX_H
#define HITBOX_H
#include <functional>
#include <glm/glm.hpp>

namespace dae
{
class GameObject;
class Hurtbox;

class Hitbox final
{
public:
	Hitbox() = default;
	Hitbox( GameObject* pParent,
			const glm::vec4 rect,
			const std::vector<size_t>& targetHashes,
			const std::function<void( GameObject*, Hurtbox* )>& callback );
	Hitbox( const glm::vec4 rect,
			std::vector<size_t>&& targetHashes,
			const std::function<void( GameObject*, Hurtbox* )>& callback );
	~Hitbox();

	const std::vector<size_t>& GetTargets() const;
	GameObject* GetParent() const;

	void OnCollision( Hurtbox* pOther );

	Hitbox( const Hitbox& ) = delete;
	Hitbox( Hitbox&& ) = delete;
	Hitbox& operator=( const Hitbox& ) = delete;
	Hitbox& operator=( Hitbox&& ) = delete;

	// Trivial get/set
	glm::vec4 m_Rectangle{};

private:
	GameObject* m_pParent{};
	std::vector<size_t> m_TargetHashes{};
	std::function<void( GameObject*, Hurtbox* )> m_Callback{};
};
} // namespace dae
#endif
