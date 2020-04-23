#include "composite.h"

#include <sstream>
#include <utility>

namespace my
{

IFood::IFood(std::string name)
    : m_name(std::move(name))
{
}

std::string IFood::name() const
{
    return m_name;
}

std::string IFood::to_string() const
{
    return to_string_impl(0);
}

void FoodMenu::add(const FoodMenu::Ptr& ptr)
{
    m_children.emplace_back(ptr);
}

void FoodMenu::remove(const FoodMenu::Ptr& ptr)
{
    m_children.remove(ptr);
}

const std::list<IFood::Ptr> FoodMenu::get() const
{
    return m_children;
}

std::string FoodMenu::to_string_impl(unsigned int offset) const
{
    std::string offset_str(offset, ' ');
    std::ostringstream stream;
    stream << name() << ":\n\n";
    for (const Ptr& elem : m_children)
        stream << offset_str << "- " << elem->to_string_impl(offset + 2) << '\n';
    return stream.str();
}

FoodElement::FoodElement(std::string name, unsigned int price)
    : IFood(std::move(name))
    , m_price(price)
{
}

void FoodElement::add(const Ptr& /* ptr */)
{
    throw std::runtime_error("Can't add to FoodElement");
}

void FoodElement::remove(const Ptr& /* ptr */)
{
    throw std::runtime_error("Can't remove from FoodElement");
}

unsigned int FoodElement::price() const
{
    return m_price;
}

std::string FoodElement::to_string_impl(unsigned int /* offset */) const
{
    unsigned int dollars = m_price / 100, cents = m_price - dollars * 100;
    std::string ret = name() + "\t\t$" + std::to_string(dollars) + '.';
    if (cents == 0)
        ret += "00";
    else if (cents < 10)
        ret += "0" + std::to_string(cents);
    else
        ret += std::to_string(cents);
    return ret;
}

} // namespace my
