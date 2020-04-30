#include "command.h"
#include "receiver.h"

#include <stdexcept>
#include <iostream>

namespace my
{

Receiver* Command::receiver() const
{
    return m_receiver;
}

Command::Command(Receiver* receiver)
    : m_receiver(receiver)
{
    if (m_receiver == nullptr)
        throw std::invalid_argument("Receiver can not be null!");
}


IncrementCommand::IncrementCommand(Receiver* receiver, std::size_t index)
    : Command(receiver)
    , m_index(index)
{
}

void IncrementCommand::execute() const
{
    std::cerr << "Increment element number " << m_index << "...\n";
    receiver()->set_element(m_index, receiver()->get_element(m_index) + 1);
}

void IncrementCommand::undo() const
{
    std::cerr << "Undo increment element number " << m_index << "...\n";
    receiver()->set_element(m_index, receiver()->get_element(m_index) - 1);
}


ZeroCommand::ZeroCommand(Receiver* receiver, std::size_t index)
    : Command(receiver)
    , m_index(index)
    , m_backup(this->receiver()->get_element(m_index))
{
}

void ZeroCommand::execute() const
{
    std::cerr << "Zero element number " << m_index << "...\n";
    m_backup = receiver()->get_element(m_index);
    receiver()->set_element(m_index, 0);
}

void ZeroCommand::undo() const
{
    std::cerr << "Undo zero element number " << m_index << "...\n";
    receiver()->set_element(m_index, m_backup);
}

} // namespace my
