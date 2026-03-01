#include "TextureComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"

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
