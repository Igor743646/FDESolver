#pragma once
#include <iostream>

namespace NPrinter {

    template<class T, class... Args>
    void Print(T&&, Args...);

    template<class T>
    void Print(T&&);
}