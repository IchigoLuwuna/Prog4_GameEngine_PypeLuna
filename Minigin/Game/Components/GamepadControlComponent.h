#ifndef GAMEPADCONTROLCOMPONENT_H
#define GAMEPADCONTROLCOMPONENT_H
#include <Patterns.h>
#include <Components.h>

namespace dae
{
class GamepadControlComponent : public Component
{
public:
	GamepadControlComponent( GameObject* pParent, float speed );

	virtual void Update() override;

	void DoLeftAxis();
	void DoRightAxis();

private:
	ReferencePtr<TransformComponent> m_TransformRef{};
	float m_Speed{};
};
} // namespace dae
#endif
