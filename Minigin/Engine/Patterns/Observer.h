#ifndef OBSERVER_H
#define OBSERVER_H
#include <vector>
#include <Memory.h>

namespace dae
{
class Observer
{
public:
	virtual ~Observer() = default;
	virtual void Notify( size_t eventHash, void* pSubject ) = 0;
};

template <typename SubjectType>
class Messenger final
{
public:
	Messenger( SubjectType* pParent )
		: m_pSubject( pParent )
	{
	}
	~Messenger() = default;

	template <typename T>
	void RegisterObserver( ReferencePtr<T> pObserver )
	{
		m_Observers.push_back( { pObserver.Get(), Validator{ pObserver.GetControlBlock() } } );
	}
	void RemoveObserver( Observer* pObserver )
	{
		m_Observers.erase( std::ranges::find_if(
			m_Observers.begin(), m_Observers.end(), [&]( auto& observer ) { return observer.first == pObserver; } ) );
	}

	void NotifyObservers( size_t eventHash )
	{
		for ( auto& observer : m_Observers )
		{
			if ( !observer.second.Validate() )
			{
				return;
			}
			observer.first->Notify( eventHash, m_pSubject );
		}
	}

private:
	std::vector<std::pair<Observer*, Validator>> m_Observers{};
	SubjectType* m_pSubject{};
};
} // namespace dae
#endif
