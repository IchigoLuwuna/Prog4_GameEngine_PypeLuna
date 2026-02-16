#include "Timer.h"
#include <chrono>

using namespace dae;

Timer::Timer()
	: m_Start{ std::chrono::high_resolution_clock::now() }
	, m_T1{ m_Start }
	, m_T2{ m_Start }
{
}

void Timer::Lap()
{
	m_T1 = m_T2;
	m_T2 = std::chrono::high_resolution_clock::now();

	m_DeltaTime = std::chrono::duration<float>( m_T2 - m_T1 ).count();
}

void Timer::Reset()
{
	m_Start = std::chrono::high_resolution_clock::now();
	m_T1 = std::chrono::high_resolution_clock::now();
	m_T2 = std::chrono::high_resolution_clock::now();
	m_DeltaTime = 0.f;
}

float Timer::GetElapsed() const
{
	return m_DeltaTime;
}

float Timer::GetTotalElapsed() const
{
	return std::chrono::duration<float>( std::chrono::high_resolution_clock::now() - m_Start ).count();
}
