#include "Component.h"
#include "Engine/Core/Minigin.h"
#include "Engine/Helpers/SdbmHash.h"

dae::Component::~Component()
{
	Minigin::eventManager.SendEvent( { Hash( "e_ComponentRemoved" ), this } );
}
