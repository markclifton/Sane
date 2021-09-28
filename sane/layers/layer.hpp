#pragma once

#include <string>
#include <vector>

namespace Sane
{
    namespace Layers
    {
        class Stack;
    }

    class Layer
    {
        friend class Layers::Stack;
    public:
        Layer(const std::string& name);

        virtual void Update() {}

        virtual void OnAttach() {}
        virtual void OnDetach() {}

        const std::string& Name() const { return name; }

    protected:
        std::string name;
    };
}