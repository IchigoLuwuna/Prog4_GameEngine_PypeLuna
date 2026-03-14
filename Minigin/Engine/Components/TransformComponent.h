#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include <glm/glm.hpp>
#include "Engine/Patterns/Component.h"

namespace dae
{
class TransformComponent : public Component
{
public:
	TransformComponent( GameObject* pParent )
		: Component( pParent )
	{
	}

	TransformComponent( GameObject* pParent, const glm::vec2& pos )
		: Component( pParent )
		, m_WorldTransform( pos )
	{
	}

	TransformComponent( GameObject* pParent, float x, float y )
		: Component( pParent )
		, m_WorldTransform( x, y )
	{
	}

	virtual void Update() override
	{
	}

	const glm::vec2& GetPosition();

	void MoveTo( float x, float y );
	void MoveTo( const glm::vec2& position );
	void Move( float x, float y );
	void Move( const glm::vec2& movement );

	void MarkForUpdate();

private:
	glm::vec2 m_WorldTransform{};
	glm::vec2 m_LocalTransform{};

	mutable bool m_WorldDirty{ true };
};
} // namespace dae

#endif
