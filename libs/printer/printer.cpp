#include <printer.h>

template<class T, class... Args>
void NPrinter::Print(T&& object, Args... tail) {
    std:cout << object << ' ';
    Print(std::forward<Args>(tail...));
}

template<class T>
void NPrinter::Print(T&& object) {
    std:cout << object << std::endl;
}