#ifndef VALIDATOR_H
#define VALIDATOR_H
#include "Memory/ControlBlock.h"

namespace dae
{
class Validator
{
public:
	template <typename T>
	Validator( ControlBlock<T>* pControlBlock )
		: m_pControlBlock( reinterpret_cast<ControlBlock<uint8_t>*>( pControlBlock ) )
	{
		m_pControlBlock->AddReference();
	}
	~Validator()
	{
		if ( !m_pControlBlock )
		{
			return;
		}
		m_pControlBlock->RemoveReference();
	}

	Validator( const Validator& other )
	{
		m_pControlBlock = other.m_pControlBlock;
		if ( m_pControlBlock )
		{
			m_pControlBlock->AddReference();
		}
	}
	Validator& operator=( const Validator& other )
	{
		if ( &other == this )
		{
			return *this;
		}

		if ( m_pControlBlock )
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

	void* Get()
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

private:
	ControlBlock<uint8_t>* m_pControlBlock{}; // Has to have a type even though we never interact with it
};
} // namespace dae
#endif
