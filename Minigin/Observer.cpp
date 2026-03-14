#include "Observer.h"
#include <algorithm>

template <typename SubjectType>
void dae::Subject<SubjectType>::NotifyObservers( size_t eventHash )
{
	for ( auto* observer : m_Observers )
	{
		observer->Notify( eventHash, reinterpret_cast<SubjectType*>( this ) );
	}
}
