#ifndef CALLABLE_HPP
#define CALLABLE_HPP
#include <iostream>

struct Callable{
    virtual ~Callable() = default;
    virtual void Run() = 0;
};

#endif