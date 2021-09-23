#pragma once

namespace Sane
{
    class App
    {
    public:
        App();
        virtual ~App() = default;
        virtual void Run() = 0;
    };

    App* CreateApp();
}