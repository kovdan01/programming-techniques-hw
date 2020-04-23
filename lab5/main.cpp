#include "composite.h"

#include <iostream>
#include <stdexcept>
#include <memory>

int main() try
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    my::IFood::Ptr menu        = std::make_shared<my::FoodMenu>("Menu");

    my::IFood::Ptr sandwitches = std::make_shared<my::FoodMenu>("Sandwitches");
    my::IFood::Ptr desserts    = std::make_shared<my::FoodMenu>("Desserts");

    my::IFood::Ptr pork        = std::make_shared<my::FoodElement>("Pork", 210);
    my::IFood::Ptr beef        = std::make_shared<my::FoodElement>("Beef", 350);
    my::IFood::Ptr cheese      = std::make_shared<my::FoodElement>("Cheese", 180);

    my::IFood::Ptr ice_cream   = std::make_shared<my::FoodElement>("Ice cream", 300);
    my::IFood::Ptr fruit_cake  = std::make_shared<my::FoodElement>("Fruit cake", 405);

    sandwitches->add(std::move(pork));
    sandwitches->add(std::move(beef));
    sandwitches->add(std::move(cheese));

    desserts->add(std::move(ice_cream));
    desserts->add(std::move(fruit_cake));

    menu->add(std::move(sandwitches));
    menu->add(std::move(desserts));

    std::cout << menu->to_string();
}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
