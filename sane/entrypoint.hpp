#pragma once

#include <sane/core/app.hpp>

#include <filesystem>
#include <string>

extern Sane::App* CreateApp();

int main(int argc, char* argv[])
{
    std::string path(argv[0]);
    while (!path.empty() && path.back() != '/')
        path.pop_back();

    std::filesystem::current_path(path);

    auto app = Sane::CreateApp();
    app->Run();
    delete app;

    return 0;
}