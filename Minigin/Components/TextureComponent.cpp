#include "TextureComponent.h"
#include "Engine/Patterns/RenderComponent.h"
#include "Engine/Patterns/GameObject.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Core/ResourceManager.h"

dae::TextureComponent::TextureComponent( GameObject* pParent, const std::string& path )
	: RenderComponent( pParent )
	, m_Texture{ ResourceManager::GetInstance().LoadTexture( path ) }
{
}

void dae::TextureComponent::Render() const
{
	TransformComponent* position{ GetParent()->GetComponent<TransformComponent>() };
	Renderer::GetInstance().RenderTexture( *m_Texture, position->GetPosition().x, position->GetPosition().y );
}
