#ifndef BRAINCOMPONENT_H
#define BRAINCOMPONENT_H
#include <Patterns.h>
#include "Game/Classes/HiveMind.h"

namespace dae
{
class BrainComponent : public Component
{
public:
	BrainComponent( GameObject* pParent, HiveMind::HiveMindType type );
	virtual ~BrainComponent();

	virtual void Update() override { };

	glm::vec2 GetFormationPosition() const;
	GameObject* GetPlayer();

private:
	HiveMind m_HiveMind{};
	uint64_t m_FormationSlot{};
};
} // namespace dae
#endif
