#ifndef PIXELTEXTCOMPONENT_H
#define PIXELTEXTCOMPONENT_H
#include "Game/Components/TextComponent.h"
#include <Helpers.h>

namespace dae
{
class PixelTextComponent : public TextComponent
{
public:
	PixelTextComponent( GameObject* pParent,
						const std::string& path,
						const std::string& mapping,
						const glm::vec2& dimensions );
	PixelTextComponent( GameObject* pParent,
						const std::string& path,
						std::string&& mapping,
						const glm::vec2& dimensions );

	virtual ~PixelTextComponent() = default;

	virtual void Update() override
	{
		; // No behaviour (; for auto-formatting)
	};
	virtual void Render() const override;

	virtual glm::vec2 GetSize() const override;

	virtual void SetText( const std::string& text ) override;
	virtual void SetColor( const SDL_Color& ) override
	{
		// No behaviour
	}

	PixelTextComponent& SetIgnore( bool ignore );

private:
	PixelFont m_Font;
	std::string m_Text{};
	mutable glm::vec2 m_Size{};
	mutable bool m_SizeDirty{};
};
} // namespace dae
#endif
