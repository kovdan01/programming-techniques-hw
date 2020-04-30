#include "invoker.h"
#include "command.h"
#include "receiver.h"

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

int main() try
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::unique_ptr<my::Receiver> receiver = std::make_unique<my::Receiver>();
    my::Invoker invoker;

    std::vector<std::shared_ptr<my::Command>> increments(my::Receiver::DEFAULT_SIZE);
    for (std::size_t i = 0; i < increments.size(); ++i)
        increments[i] = std::make_shared<my::IncrementCommand>(receiver.get(), i);

    std::vector<std::shared_ptr<my::Command>> zeros(my::Receiver::DEFAULT_SIZE);
    for (std::size_t i = 0; i < zeros.size(); ++i)
        zeros[i] = std::make_shared<my::ZeroCommand>(receiver.get(), i);

    invoker.push_command(increments[5]);
    receiver->print();
    invoker.push_command(increments[6]);
    receiver->print();
    invoker.pop_command();
    receiver->print();
    invoker.push_command(increments[5]);
    receiver->print();
    invoker.push_command(increments[7]);
    receiver->print();
    invoker.push_command(zeros[5]);
    receiver->print();
    invoker.pop_command();
    receiver->print();
    invoker.pop_command();
    receiver->print();
    invoker.pop_command();
    receiver->print();
}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
