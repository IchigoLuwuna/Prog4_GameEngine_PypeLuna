#ifndef HITBOXREGISTRY_H
#define HITBOXREGISTRY_H
#include <unordered_map>
#include <vector>
#include <glm/fwd.hpp>
#include "Patterns/Singleton.h"

namespace dae
{
class Hitbox;
class Hurtbox;

class HitboxRegistry : public Singleton<HitboxRegistry>
{
public:
	void DoCollisions();

	void RegisterHitbox( Hitbox* pHitbox );
	void RemoveHitbox( const Hitbox* pHitbox );

	void RegisterHurtbox( Hurtbox* pHurtbox, size_t targetHash );
	void RemoveHurtbox( const Hurtbox* pHurtbox, size_t targetHash );

private:
	std::vector<Hitbox*> m_RegisteredHitboxes{};
	std::unordered_map<size_t, std::vector<Hurtbox*>> m_RegisteredHurtboxes{};

	bool Collides( const glm::vec4& a, const glm::vec4& b );

	friend class Singleton<HitboxRegistry>;
	HitboxRegistry() = default;
};
} // namespace dae
#endif
