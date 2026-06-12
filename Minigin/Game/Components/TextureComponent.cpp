#include "TextureComponent.h"
#include <Core.h>
#include <Rendering.h>
#include <Components.h>

dae::TextureComponent::TextureComponent( GameObject* pParent, const std::string& path )
	: RenderComponent( pParent )
	, m_Texture{ ResourceManager::GetInstance().LoadTexture( path ) }
{
}

void dae::TextureComponent::Render() const
{
	auto position{ GetParent()->GetComponent<TransformComponent>() };
	Renderer::GetInstance().RenderTexture( *m_Texture, position->GetPosition().x, position->GetPosition().y );
}
