#ifndef SPRITESHEETCOMPONENT_H
#define SPRITESHEETCOMPONENT_H
#include <Patterns.h>
#include <Helpers.h>

namespace dae
{
class SpriteSheetComponent final : public RenderComponent
{
public:
	SpriteSheetComponent( GameObject* pParent, const std::string& path, const SpriteSheet::SpriteSheetInfo& info );

	virtual void Update() override { };
	virtual void Render() const override;

	void SetIndex( uint32_t index );
	void SetIndex( uint32_t column, uint32_t row );

private:
	SpriteSheet m_Sheet;
};
} // namespace dae
#endif
