#pragma once

namespace Sane
{
    namespace Components
    {
        struct Visible
        {
            Visible(bool visible = false)
                : visible(visible)
            {}

            bool visible;
        };
    }
}