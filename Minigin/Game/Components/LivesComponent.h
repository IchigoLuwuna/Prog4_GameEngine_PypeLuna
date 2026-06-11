#ifndef LIVESCOMPONENT_H
#define LIVESCOMPONENT_H
#include <Patterns.h>

namespace dae
{
class LivesComponent : public Component
{
public:
	LivesComponent( GameObject* pParent, uint32_t startingLives );

	virtual void Update() override { };

	uint32_t GetLives() const;

	void SetLives( uint32_t newAmount );
	void ReduceLives( uint32_t amount );
	void IncreaseLives( uint32_t amount );

private:
	uint32_t m_Lives{};
};
} // namespace dae
#endif
