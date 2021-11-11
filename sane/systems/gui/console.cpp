#include "sane/systems/gui/console.hpp"

namespace Sane
{
    Console::Console()
        : System("Console")
        , sink_(Logging::GetSink())
    {
    }

    void Console::RenderGui()
    {
        sink_->Render();
    }
}