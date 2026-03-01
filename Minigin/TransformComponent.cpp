#include "TransformComponent.h"
#include "GameObject.h"

const glm::vec3& dae::TransformComponent::GetPosition()
{
	if ( !m_WorldDirty )
	{
		return m_WorldTransform;
	}

	GameObject* pParentGameObject{ GetParent()->GetParent() };
	glm::vec3 parentPos{};
	if ( pParentGameObject )
	{
		// Recurse up the scene graph
		parentPos = pParentGameObject->GetComponent<TransformComponent>()->GetPosition();
	}

	m_WorldTransform = m_LocalTransform + parentPos;
	m_WorldDirty = false;

	return m_WorldTransform;
}

void dae::TransformComponent::MoveTo( const float x, const float y, const float z )
{
	m_LocalTransform.x = x;
	m_LocalTransform.y = y;
	m_LocalTransform.z = z;
	MarkForUpdate();
}

void dae::TransformComponent::MoveTo( const glm::vec3& position )
{
	m_LocalTransform = position;
	MarkForUpdate();
}

void dae::TransformComponent::Move( float x, float y, float z )
{
	m_LocalTransform += glm::vec3{ x, y, z };
	MarkForUpdate();
}

void dae::TransformComponent::Move( const glm::vec3& movement )
{
	m_LocalTransform += movement;
	MarkForUpdate();
}

void dae::TransformComponent::MarkForUpdate()
{
	m_WorldDirty = true;

	// Recurse down scene graph
	int childCount{ GetParent()->GetChildCount() };
	for ( int index{}; index < childCount; ++index )
	{
		GetParent()->GetChildAt( index )->GetComponent<dae::TransformComponent>()->MarkForUpdate();
	}
}
