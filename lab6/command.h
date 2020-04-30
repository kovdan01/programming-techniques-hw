#ifndef COMMAND_H
#define COMMAND_H

#include "receiver.h"

namespace my
{

class Command
{
public:
    explicit Command(Receiver* receiver);
    virtual ~Command() = default;
    virtual void execute() const = 0;
    virtual void undo() const = 0;

protected:
    Receiver* receiver() const;

private:
    Receiver* m_receiver;
};


class IncrementCommand : public Command
{
public:
    IncrementCommand(Receiver* receiver, std::size_t index);
    void execute() const override;
    void undo() const override;

private:
    std::size_t m_index;
};

class ZeroCommand : public Command
{
public:
    ZeroCommand(Receiver* receiver, std::size_t index);
    void execute() const override;
    void undo() const override;

private:
    std::size_t m_index;
    mutable std::uint32_t m_backup;
};

} // namespace my

#endif // COMMAND_H
