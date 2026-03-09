#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "Singleton.h"

namespace dae
{
class InputManager final : public Singleton<InputManager>
{
public:
	bool ProcessInput();
};
} // namespace dae
#endif
