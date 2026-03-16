#include "../include/App.hpp"
/*
 *  cmake -S . -B build-make -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
    cmake --build build-make -j
    ./build-make/src/prog
 *
 * */
int main()
{
    App app;
    if (!app.initialize())
    {
        return 1;
    }

    app.run();
    app.shutdown();
    return 0;
}
