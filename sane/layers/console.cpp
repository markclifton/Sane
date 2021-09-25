#include "sane/layers/console.hpp"

namespace Sane
{
    Console::Console()
        : Layer("Console")
    {
    }

    void Console::Render()
    {
        logSink_->Render();
    }
}