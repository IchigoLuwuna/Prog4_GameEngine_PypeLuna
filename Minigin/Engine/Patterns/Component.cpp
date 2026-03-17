#include "Component.h"
#include "Engine/Core/EventManager.h"
#include "Engine/Helpers/SdbmHash.h"

dae::Component::~Component()
{
	EventManager::GetInstance().SendEvent( { Hash( "e_ComponentRemoved" ), this } );
}
