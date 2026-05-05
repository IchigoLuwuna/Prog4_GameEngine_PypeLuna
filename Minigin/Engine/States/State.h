#ifndef STATE_H
#define STATE_H

namespace dae
{
class State
{
public:
	State( void* pParent )
		: m_pParent( pParent )
	{
	}

	void* GetParent()
	{
		return m_pParent;
	}

private:
	void* m_pParent{};
};
} // namespace dae
#endif
