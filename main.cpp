#include <iostream>
#include "src/include.hpp"

int main ()
{

    Var<int> a (42, "a");
    Var<int> b (32, "b");

    b = a;
}