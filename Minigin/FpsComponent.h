#ifndef FPSCOMPONENT_H
#define FPSCOMPONENT_H
#include "Component.h"

namespace dae
{
class FpsComponent : public Component
{
public:
	FpsComponent( GameObject* pParent )
		: Component( pParent )
	{
	}
	virtual ~FpsComponent() = default;

	virtual void Update() override;

private:
	float m_Fps{};
	float m_LastUpdate{};
};
} // namespace dae
#endif
