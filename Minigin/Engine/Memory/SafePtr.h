#ifndef SAFEPTR_H
#define SAFEPTR_H
#include "ControlBlock.h"

// A safe pointer creates a control block upon initialisation
// See ReferenceControlBlock.h for details on its lifetime management
namespace dae
{
template <typename T>
class SafePtr final
{
public:
	SafePtr() = default;
	SafePtr( const SafePtr& ) = delete;
	SafePtr& operator=( const SafePtr& ) = delete;

	template <typename... Args>
	SafePtr( const Args&... args )
		: m_pControlBlock( new ControlBlock<T>( std::make_unique<T>( args... ) ) )
	{
	}

	SafePtr( std::unique_ptr<T>&& pData )
		: m_pControlBlock( new ControlBlock<T>( std::move( pData ) ) )
	{
	}

	template <typename Derived>
		requires std::derived_from<Derived, T>
	SafePtr( SafePtr<Derived>&& other )
	{
		auto pOtherAsBase{ reinterpret_cast<SafePtr<T>*>( &other ) };

		m_pControlBlock = pOtherAsBase->m_pControlBlock;
		pOtherAsBase->m_pControlBlock = nullptr;
	}

	SafePtr( SafePtr&& other )
	{
		m_pControlBlock = other.m_pControlBlock;
		other.m_pControlBlock = nullptr;
	}
	SafePtr& operator=( SafePtr&& other )
	{
		if ( &other == this )
		{
			return *this;
		}

		m_pControlBlock = other.m_pControlBlock;
		other.m_pControlBlock = nullptr;

		return *this;
	}

	~SafePtr()
	{
		if ( m_pControlBlock )
		{
			m_pControlBlock->Reset();
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

	ControlBlock<T>* GetControlPtr() const
	{
		return m_pControlBlock;
	}

	T* Get() const
	{
		return m_pControlBlock->Get();
	}

	size_t GetRefCount()
	{
		return m_pControlBlock->GetRefCount();
	}

private:
	ControlBlock<T>* m_pControlBlock{};
};
} // namespace dae
#endif
