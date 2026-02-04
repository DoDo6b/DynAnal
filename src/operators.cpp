#include <iostream>

#include "../include.hpp"


template <typename T>
template <typename RT>
Var<T>& Var<T>::operator= (const RT& b)
{
    this->var = b;
    std::cout << "var " << this->name << ": " << this->var << std::endl;
    return *this;
}

template Var<int>& Var<int>::operator=<int>(const int&);