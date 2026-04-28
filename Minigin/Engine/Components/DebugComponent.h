#ifndef DEBUGCOMPONENT_H
#define DEBUGCOMPONENT_H
#include "Patterns/Component.h"
#include <iostream>
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
	virtual ~DebugComponent()
	{
		std::cout << "Removing: " << m_Name << "\n";
	}

	virtual void Update() override
	{
	}

private:
	std::string m_Name{};
};
} // namespace dae
#endif
