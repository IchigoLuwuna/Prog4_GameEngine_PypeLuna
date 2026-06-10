#ifndef ENEMY_FUNCTIONS_H
#define ENEMY_FUNCTIONS_H
#include <memory>
#include <Patterns.h>

namespace dae::functions::enemy
{
std::unique_ptr<GameObject> MakeZako();
std::unique_ptr<GameObject> MakeGoei();
} // namespace dae::functions::enemy
#endif
