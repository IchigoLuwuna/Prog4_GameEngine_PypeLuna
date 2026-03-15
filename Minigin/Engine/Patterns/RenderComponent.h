#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H
#include "Component.h"

namespace dae
{
class RenderComponent : public Component
{
public:
	RenderComponent( GameObject* pParent )
		: Component( pParent )
	{
	}

	virtual ~RenderComponent() = default;

	virtual void Render() const = 0;
};
} // namespace dae

#endif
