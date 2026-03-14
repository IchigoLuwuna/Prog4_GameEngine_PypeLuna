#ifndef OBSERVER_H
#define OBSERVER_H
#include <concepts>
#include <vector>

// TODO:Abstract away all the messy template stuff
namespace dae
{
class Subject;
class ObserverBase
{
public: // Names are a bit different to support templates w/o naming conflicts on the user end
	virtual ~ObserverBase() = default;
	virtual void ForwardNotify( size_t eventHash, Subject* pSubject ) = 0;
};

template <typename SubjectType>
	requires std::derived_from<SubjectType, Subject>
class Observer : public ObserverBase
{
public:
	virtual ~Observer() = default;
	virtual void ForwardNotify( size_t eventHash, Subject* pSubject ) override final
	{
		Notify( eventHash, reinterpret_cast<SubjectType*>( pSubject ) );
	}

protected:
	virtual void Notify( size_t eventHash, SubjectType* pSubject ) = 0;
};

class Subject
{
public:
	virtual ~Subject() = default;
	void RegisterObserver( ObserverBase* pObserver );
	void RemoveObserver( ObserverBase* pObserver );

protected:
	void NotifyObservers( size_t eventHash );

private:
	std::vector<ObserverBase*> m_Observers{};
};
} // namespace dae
#endif
