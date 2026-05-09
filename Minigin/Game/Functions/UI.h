#ifndef UI_FUNCTIONS_H
#define UI_FUNCTIONS_H
#include <memory>
#include <Patterns.h>

namespace dae::functions::ui
{
std::unique_ptr<GameObject> MakeBackground();
std::unique_ptr<GameObject> MakeFpsCounter();
std::unique_ptr<GameObject> MakeScoreboard( GameObject* pPlayer );
} // namespace dae::functions::ui
#endif
