#ifndef TIMER_H
#define TIMER_H
#include <chrono>
#include "Singleton.h"

namespace dae
{
class Timer final : public Singleton<Timer>
{
public:
	// Methods
	void Lap();
	void Reset();

	// Getters
	float GetElapsed() const;
	float GetTotalElapsed() const;

private:
	// std::chrono::time_point<std::chrono::high_resolution_clock> m_Start{};
	// std::chrono::time_point<std::chrono::high_resolution_clock> m_T1{};
	// std::chrono::time_point<std::chrono::high_resolution_clock> m_T2{};
	std::chrono::time_point<std::chrono::steady_clock> m_Start{};
	std::chrono::time_point<std::chrono::steady_clock> m_T1{};
	std::chrono::time_point<std::chrono::steady_clock> m_T2{};

	float m_DeltaTime{};

	friend class dae::Singleton<Timer>;
	Timer();
};
} // namespace dae
#endif
