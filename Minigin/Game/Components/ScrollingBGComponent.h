#ifndef SCROLLINGBGCOMPONENT_H
#define SCROLLINGBGCOMPONENT_H
#include <Patterns.h>
#include <Helpers.h>

namespace dae
{
class ScrollingBGComponent : public RenderComponent
{
public:
	enum class ScrollingDir
	{
		up,
		down,
		left,
		right,
	};

	ScrollingBGComponent( GameObject* pParent, const std::string& path, float speed, ScrollingDir direction );

	virtual void Update() override;
	virtual void Render() const override;

private:
	ReferencePtr<Texture2D> m_BackgroundTex;
	glm::vec2 m_CurrentScroll{};
	glm::vec2 m_ScrollingVelocity{};
	ScrollingDir m_Direction{};
};
} // namespace dae
#endif
