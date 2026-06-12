#include "Random.h"
#include <cassert>
#include <random>

namespace dae::random
{
// Not in header as this is meant to be private
std::default_random_engine& GetRandomEngine();
} // namespace dae::random

bool dae::random::GetRand( float chance )
{
	assert( chance >= 0.f && chance <= 1.f && "Chance must be between 0.f and 1.f" );
	std::uniform_real_distribution distribution{ 0.f, 1.f };
	return distribution( GetRandomEngine() ) <= chance;
}

bool dae::random::GetRand( int chance )
{
	assert( chance >= 0 && chance <= 100 && "Chance must be between 0 and 100" );
	std::uniform_int_distribution distribution{ 0, 100 };
	return distribution( GetRandomEngine() ) <= chance;
}

float dae::random::GetRand( float min, float max )
{
	std::uniform_real_distribution distribution{ min, max };
	return distribution( GetRandomEngine() );
}

int dae::random::GetRand( int min, int max )
{
	std::uniform_int_distribution distribution{ min, max };
	return distribution( GetRandomEngine() );
}

std::default_random_engine& dae::random::GetRandomEngine()
{
	// Thread local as the random functions might be used on multiple threads
	thread_local std::random_device randomDevice{};
	thread_local std::default_random_engine randomEngine{ randomDevice() };
	return randomEngine;
}
