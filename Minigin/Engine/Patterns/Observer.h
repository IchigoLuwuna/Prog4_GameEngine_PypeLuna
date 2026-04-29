#ifndef OBSERVER_H
#define OBSERVER_H
#include <vector>

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
class Messenger final
{
public:
	Messenger( SubjectType* pParent )
		: m_pSubject( pParent )
	{
	}
	~Messenger() = default;

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
			observer->Notify( eventHash, m_pSubject );
		}
	}

private:
	std::vector<Observer<SubjectType>*> m_Observers{};
	SubjectType* m_pSubject{};
};
} // namespace dae
#endif
