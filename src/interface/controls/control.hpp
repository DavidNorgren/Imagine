#pragma once

#include <functional>

#include "base.hpp"
using namespace Base;

#include "interface/element.hpp"
#include "interface/controls/icons.hpp"
#include "interface/images.hpp"
#include "settings/settings.hpp"


#define TRUE_FUNC   []() { return true; }
#define FALSE_FUNC  []() { return false; }

#define CONTROL_MARGIN_RIGHT   2
#define CONTROL_MARGIN_LEFT    2
#define CONTROL_MARGIN_BOTTOM  8


namespace Mineimator
{

    /* Interface for all the controls. */
    class Control : public Element
    {
        public:
            /* Element methods. */
            void mouseEvent() override;
            void keyEvent() override;
            void setParent(Element* parent) override;
            
            string name;
            
        protected:
            std::function<void()> clickFunc = nullptr;
            std::function<bool()> visibleFunc;
            bool mouseOn = false, pressed = false;
            
    };
    
}