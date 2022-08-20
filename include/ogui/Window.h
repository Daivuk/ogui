#pragma once

#include "types.h"
#include <memory>
#include <string>
#include <vector>

namespace ogui
{
    class Control;

    using ControlRef = std::shared_ptr<Control>;

    class Window final
    {
    public:
        Window();
        ~Window();

        void setName(const std::string &name);
        const std::string &getName() const { return m_name; }

        void clear();
        void add(const ControlRef &pControl);
        void remove(const ControlRef &pControl);

    private:
        std::string m_name = "Panel";
        Rect m_rect = { 0.0f, 0.0f, 200.0f, 200.0f };
        std::vector<ControlRef> m_controls;
    };
}
