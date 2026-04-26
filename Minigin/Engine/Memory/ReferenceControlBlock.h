#ifndef REFERENCECONTROLBLOCK_H
#define REFERENCECONTROLBLOCK_H
#include <memory>

// The control block from is a self-managing object, meaning that it controls its own lifetime after construction
// If m_pData is nullptr (meaning the original SafePtr is gone) AND m_RefCount is 0, this object will self-destruct
namespace dae
{
template <typename T>
class ReferenceControlBlock final
{
public:
	ReferenceControlBlock( std::unique_ptr<T>&& pData )
		: m_pData( std::move( pData ) )
	{
	}

	T* Get()
	{
		return m_pData.get();
	}

	size_t GetRefCount()
	{
		return m_RefCount;
	}

	// For SafePtr
	void Reset()
	{
		m_pData.reset();
		SelfDestructIfPossible();
	}

	// For ReferencePtr
	void AddReference()
	{
		++m_RefCount;
	}
	void RemoveReference()
	{
		--m_RefCount;
		SelfDestructIfPossible();
	}
	bool Validate()
	{
		return m_pData != nullptr;
	}

private:
	std::unique_ptr<T> m_pData{};
	size_t m_RefCount{};

	// For itself
	void SelfDestructIfPossible()
	{
		if ( m_pData || m_RefCount != 0 )
		{
			return;
		}

		delete this;
	}
};
} // namespace dae
#endif
