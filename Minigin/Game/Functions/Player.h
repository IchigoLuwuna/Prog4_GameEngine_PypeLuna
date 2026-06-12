#ifndef PLAYER_FUNCTIONS_H
#define PLAYER_FUNCTIONS_H
#include <memory>
#include <Patterns.h>

namespace dae::functions::player
{
std::unique_ptr<GameObject> MakePlayer();

void BindInputForPlayer( GameObject* pPlayer );
void BindScoreboardForPlayer( GameObject* pPlayer );
} // namespace dae::functions::player
#endif
