#ifndef RANDOM_H
#define RANDOM_H
namespace dae::random
{
bool GetRand( float chance );
bool GetRand( int chance );
float GetRand( float min, float max );
int GetRand( int min, int max );
} // namespace dae::random
#endif
