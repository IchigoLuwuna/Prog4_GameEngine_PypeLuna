#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H
#include <memory>
#include "RenderComponent.h"
#include "Texture2D.h"

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
	std::shared_ptr<Texture2D> m_Texture;
};
} // namespace dae
#endif
