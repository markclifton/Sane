#include "sane/layers/console.hpp"

namespace Sane
{
    void Console::Process()
    {
        logSink_->Render();
    }
}