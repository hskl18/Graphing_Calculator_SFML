#include "includes/engine/engine.h"
#include <exception>
#include <iostream>
#include <string>
using namespace std;
int main(int argc, char* argv[])
{
    try
    {
        srand(time(NULL));
        Engine engine;
        if(argc == 2 && string(argv[1]) == "--smoke-test")
        {
            engine.smoke_test();
            return 0;
        }
        engine.run();
    }
    catch(const std::exception& error)
    {
        std::cerr << "Graphing calculator failed to start: " << error.what() << '\n';
        return 1;
    }
}
