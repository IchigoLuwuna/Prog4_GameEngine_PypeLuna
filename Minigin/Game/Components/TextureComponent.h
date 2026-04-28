#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H
#include "Engine/Memory/ReferencePtr.h"
#include "Engine/Patterns/RenderComponent.h"
#include "Engine/Helpers/Texture2D.h"

namespace dae
{
class TextureComponent : public RenderComponent
{
public:
	TextureComponent( dae::GameObject* pParent, const std::string& path );
	virtual ~TextureComponent() = default;

	virtual void Update() override
	{
	}
	virtual void Render() const override;

private:
	ReferencePtr<Texture2D> m_Texture;
};
} // namespace dae
#endif
