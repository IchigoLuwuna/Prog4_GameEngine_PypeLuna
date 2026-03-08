#ifndef IMGUIWINDOWS_H
#define IMGUIWINDOWS_H
#include "RenderComponent.h"
#include <vector>
#include <array>

namespace dae
{
namespace ImGuiComponents
{
void Exercise1Old();
void Exercise2Old();

class Exercise1Component : public RenderComponent
{
public:
	Exercise1Component( GameObject* pParent )
		: RenderComponent( pParent )
	{
	}
	virtual ~Exercise1Component() = default;

	virtual void Update() override;
	virtual void Render() const override;

private:
	std::vector<float> m_Results{};
	// mutable is ugly, but since ImGui button clicks happen during rendering, there's nothing we can do D:
	mutable int m_SampleCount{ 10 };
	mutable bool m_TakeMeasurement{};

	void Measure();
};

class Exercise2Component : public RenderComponent
{
public:
	Exercise2Component( GameObject* pParent )
		: RenderComponent( pParent )
	{
	}
	virtual ~Exercise2Component() = default;

	virtual void Update() override;
	virtual void Render() const override;

private:
	std::array<std::vector<float>, 2> m_Results{};
	mutable int m_SampleCount{ 10 };
	mutable std::array<bool, 2> m_TakeMeasurement{};

	void Measure();
};
} // namespace ImGuiComponents
} // namespace dae
#endif
