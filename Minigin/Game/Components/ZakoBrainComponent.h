#ifndef ZAKOBRAIN_H
#define ZAKOBRAIN_H
#include <Patterns.h>
#include "Game/Classes/HiveMind.h"

namespace dae
{
class ZakoBrainComponent : public Component
{
public:
	ZakoBrainComponent( GameObject* pParent );
	virtual ~ZakoBrainComponent();

	virtual void Update() override { };

	glm::vec2 GetFormationPosition() const;
	GameObject* GetPlayer();

private:
	HiveMind m_HiveMind{};
	uint64_t m_FormationSlot{};
};
} // namespace dae
#endif
