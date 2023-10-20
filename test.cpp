#include <iostream>

struct trait
{
    trait() = default;
    trait(const trait&) = default;
    ~trait() = default;
};

struct my_struct : trait
{

};

int main()
{
    trait jo = my_struct();
    jo = my_struct();
}