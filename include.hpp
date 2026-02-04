#pragma once

#include <string>

template <typename T>
class Var
{
public:
    Var(const std::string& name) : var(T()), name(name) {};

    template <typename RT>
    Var<T>& operator= (const RT& b);

private:
    T var;
    std::string name;
};

#define LET(type, name) Var<type> name = Var<type> (#name)