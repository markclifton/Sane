#pragma once

namespace Sane
{
    class App
    {
    public:
        App();
        virtual ~App();
        virtual void Run();
    };

    App* CreateApp();
}