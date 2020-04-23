#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <list>
#include <memory>
#include <string>

namespace my
{

class IFood
{
public:
    using Ptr = std::shared_ptr<IFood>;

    IFood(std::string name);

    IFood(const IFood&) = default;
    IFood& operator=(const IFood&) = default;
    IFood(IFood&&) = default;
    IFood& operator=(IFood&&) = default;

    virtual ~IFood() = default;

    std::string name() const;
    std::string to_string() const;

    virtual void add(const Ptr& ptr) = 0;
    virtual void remove(const Ptr& ptr) = 0;

    virtual std::string to_string_impl(unsigned int offset) const = 0;

private:
    std::string m_name;
};

class FoodMenu : public IFood
{
public:
    using IFood::IFood;

    void add(const Ptr& ptr) override;
    void remove(const Ptr& ptr) override;

    const std::list<Ptr> get() const;

    std::string to_string_impl(unsigned int offset) const override;

private:
    std::list<Ptr> m_children;
};

class FoodElement : public IFood
{
public:
    FoodElement(std::string name, unsigned int price);

    FoodElement(const FoodElement&) = default;
    FoodElement& operator=(const FoodElement&) = default;
    FoodElement(FoodElement&&) = default;
    FoodElement& operator=(FoodElement&&) = default;

    void add(const Ptr& ptr) override;
    void remove(const Ptr& ptr) override;

    unsigned int price() const;

    std::string to_string_impl(unsigned int offset) const override;

private:
    unsigned int m_price;
};

} // namespace my

#endif // COMPOSITE_H
