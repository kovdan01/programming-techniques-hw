#include "invoker.h"

namespace my
{

void Invoker::push_command(const std::shared_ptr<Command>& command)
{
    m_commands.push_back(command);
    m_commands.back()->execute();
}

void Invoker::push_command(std::shared_ptr<Command>&& command)
{
    m_commands.push_back(std::move(command));
    m_commands.back()->execute();
}

void Invoker::pop_command()
{
    m_commands.back()->undo();
    m_commands.pop_back();
}

} // namespace my
