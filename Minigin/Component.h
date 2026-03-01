#ifndef COMPONENT_H
#define COMPONENT_H

namespace dae
{
class GameObject;
class Component
{
public:
	Component( GameObject* pParent )
		: m_pParent{ pParent }
	{
	}
	virtual ~Component() = default;

	Component( const Component& ) = delete;
	Component( Component&& ) = default;
	Component& operator=( const Component& ) = delete;
	Component& operator=( Component&& ) = default;

	virtual void Update() = 0;

protected:
	GameObject* GetParent() const
	{
		return m_pParent;
	}

private:
	GameObject* m_pParent{};
};
} // namespace dae

#endif
