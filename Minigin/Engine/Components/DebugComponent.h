#ifndef DEBUGCOMPONENT_H
#define DEBUGCOMPONENT_H
#include "Engine/Patterns/Component.h"
#include <string>

namespace dae
{
class DebugComponent : public Component // adds a name to a GameObject for debugging
{
public:
	DebugComponent( GameObject* pParent, const std::string& name )
		: Component( pParent )
		, m_Name( name )
	{
	}
	virtual ~DebugComponent() = default;

	virtual void Update() override
	{
	}

private:
	std::string m_Name{};
};
} // namespace dae
#endif
