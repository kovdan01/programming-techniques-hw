#ifndef INVOKER_H
#define INVOKER_H

#include "command.h"

#include <vector>
#include <memory>

namespace my
{

class Invoker
{

private:
    std::vector<std::shared_ptr<Command>> m_commands;

public:
    void push_command(const std::shared_ptr<Command>& command);
    void push_command(std::shared_ptr<Command>&& command);
    void pop_command();
};

} // namespace my

#endif // INVOKER_H
