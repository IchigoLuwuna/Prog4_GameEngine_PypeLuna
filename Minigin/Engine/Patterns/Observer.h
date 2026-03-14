#ifndef OBSERVER_H
#define OBSERVER_H
#include <vector>
#include <string>

namespace dae
{
template <typename SubjectType>
class Observer
{
public:
	virtual ~Observer() = default;
	virtual void Notify( size_t eventHash, SubjectType* pSubject ) = 0;
};

template <typename SubjectType>
class Subject final
{
public:
	Subject( SubjectType* pParent )
		: m_pParent( pParent )
	{
	}
	virtual ~Subject()
	{
		static const auto subjectDestructed{ std::hash<std::string>()( "e_SubjectDestructed" ) };
		NotifyObservers( subjectDestructed );
	}

	void RegisterObserver( Observer<SubjectType>* pObserver )
	{
		m_Observers.push_back( pObserver );
	}
	void RemoveObserver( Observer<SubjectType>* pObserver )
	{
		m_Observers.erase( std::ranges::find( m_Observers.begin(), m_Observers.end(), pObserver ) );
	}

	void NotifyObservers( size_t eventHash )
	{
		for ( auto* observer : m_Observers )
		{
			observer->Notify( eventHash, reinterpret_cast<SubjectType*>( this ) );
		}
	}

private:
	std::vector<Observer<SubjectType>*> m_Observers{};
	SubjectType* m_pParent{};
};
} // namespace dae
#endif
