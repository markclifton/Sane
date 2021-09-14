#pragma once

#include "app.hpp"

extern Sane::App* CreateApp();

int main(int argc, char* argv[])
{
    auto app = Sane::CreateApp();
    app->Run();
    delete app;

    return 0;
}