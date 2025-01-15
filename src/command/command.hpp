#include <iostream>
#include <functional>

class Command {
private:
    const char* name;
    const char* description;

    std::function<void()> action;

}
