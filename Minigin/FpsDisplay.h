#ifndef FPSDISPLAY_H
#define FPSDISPLAY_H
#include "GameObject.h"
#include "TextObject.h"
#include "ResourceManager.h"

namespace dae
{
class FpsDisplay final : public GameObject
{
public:
	FpsDisplay();

	virtual void Render() const override;
	virtual void Update() override;

private:
	std::unique_ptr<TextObject> m_upText{};
	float m_Fps{};

	float m_LastUpdate{};
};
} // namespace dae
#endif
