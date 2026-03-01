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
		, m_Position( pos )
	{
	}

	TransformComponent( GameObject* pParent, float x, float y, float z = 0 )
		: Component( pParent )
		, m_Position( x, y, z )
	{
	}

	virtual void Update() override
	{
	}

	const glm::vec3& GetPosition() const
	{
		return m_Position;
	}
	void SetPosition( float x, float y, float z = 0 );
	void SetPosition( const glm::vec3& position );

private:
	glm::vec3 m_Position{};
};
} // namespace dae

#endif
