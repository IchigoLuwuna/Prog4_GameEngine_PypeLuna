#ifndef REFERENCEPTR_H
#define REFERENCEPTR_H
#include "SafePtr.h"

namespace dae
{
template <typename T>
class ReferencePtr final
{
public:
	ReferencePtr() = default;
	explicit ReferencePtr( const SafePtr<T>& safePtr )
		: m_pControlBlock( safePtr.GetControlPtr() )
	{
		if ( m_pControlBlock )
		{
			m_pControlBlock->AddReference();
		}
	}

	template <typename T2>
	explicit ReferencePtr( const SafePtr<T2>& safePtr )
		: m_pControlBlock( reinterpret_cast<ControlBlock<T>*>( safePtr.GetControlPtr() ) )
	{
		if ( m_pControlBlock )
		{
			m_pControlBlock->AddReference();
		}
	}

	ReferencePtr( const ReferencePtr& other )
		: m_pControlBlock( other.m_pControlBlock )
	{
		if ( m_pControlBlock )
		{
			m_pControlBlock->AddReference();
		}
	}
	ReferencePtr& operator=( const ReferencePtr& other )
	{
		if ( &other == this )
		{
			return *this;
		}

		if ( Validate() )
		{
			m_pControlBlock->RemoveReference();
		}

		m_pControlBlock = other.m_pControlBlock;

		if ( m_pControlBlock )
		{
			m_pControlBlock->AddReference();
		}

		return *this;
	}

	ReferencePtr( ReferencePtr&& other )
	{
		if ( &other == this )
		{
			return;
		}

		m_pControlBlock = other.m_pControlBlock;
		other.m_pControlBlock = nullptr;
	}
	ReferencePtr& operator=( ReferencePtr&& other )
	{
		if ( &other == this )
		{
			return *this;
		}

		if ( Validate() )
		{
			m_pControlBlock->RemoveReference();
		}

		m_pControlBlock = other.m_pControlBlock;
		other.m_pControlBlock = nullptr;

		return *this;
	}

	~ReferencePtr()
	{
		if ( m_pControlBlock )
		{
			m_pControlBlock->RemoveReference();
		}
	}

	T& operator*() const
	{
		return *m_pControlBlock->Get();
	}
	T* operator->() const
	{
		return m_pControlBlock->Get();
	}

	bool Validate()
	{
		if ( !m_pControlBlock )
		{
			return false;
		}

		if ( !m_pControlBlock->Validate() )
		{
			m_pControlBlock->RemoveReference();
			m_pControlBlock = nullptr;
			return false;
		}

		return true;
	}

	T* Get() const
	{
		return m_pControlBlock->Get();
	}

	ControlBlock<T>* GetControlBlock()
	{
		return m_pControlBlock;
	}

private:
	ControlBlock<T>* m_pControlBlock{};
};
} // namespace dae
#endif
