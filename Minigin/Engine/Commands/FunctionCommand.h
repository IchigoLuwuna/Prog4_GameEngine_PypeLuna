#ifndef FUNCTIONCOMMAND_H
#define FUNCTIONCOMMAND_H
#include <functional>
#include "Patterns/Command.h"

namespace dae
{
class FunctionCommand : public Command
{
public:
	FunctionCommand( std::function<void()> function );

	virtual void Execute() override;

private:
	std::function<void()> m_Function{};
};
} // namespace dae
#endif
