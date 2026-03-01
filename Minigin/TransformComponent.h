#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include <glm/glm.hpp>
#include "Component.h"

namespace dae
{
class TransformComponent : public Component
{
public:
	TransformComponent( GameObject* pParent )
		: Component( pParent )
	{
	}

	TransformComponent( GameObject* pParent, const glm::vec3& pos )
		: Component( pParent )
		, m_WorldTransform( pos )
	{
	}

	TransformComponent( GameObject* pParent, float x, float y, float z = 0 )
		: Component( pParent )
		, m_WorldTransform( x, y, z )
	{
	}

	virtual void Update() override
	{
	}

	const glm::vec3& GetPosition();

	void MoveTo( float x, float y, float z = 0 );
	void MoveTo( const glm::vec3& position );
	void Move( float x, float y, float z = 0 );
	void Move( const glm::vec3& movement );

	void MarkForUpdate();

private:
	glm::vec3 m_WorldTransform{};
	glm::vec3 m_LocalTransform{};

	mutable bool m_WorldDirty{ true };
};
} // namespace dae

#endif
