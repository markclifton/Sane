#include "sane/layers/console.hpp"

namespace Sane
{
    Console::Console()
        : Layer("Console")
        , sink_(Logging::GetSink())
    {
    }

    void Console::Update()
    {
        sink_->Render();
    }
}