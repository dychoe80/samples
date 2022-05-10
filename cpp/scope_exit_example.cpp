#include "scope_exit.h"
#include <iostream>

class Foo
{
public:
    void deleteObject() { std::cout << __FUNCTION__ << std::endl; }
};


int main()
{
    Foo foo;
    auto fooExit = make_scope_exit([&foo]() { foo.deleteObject(); });
    return 0;
}