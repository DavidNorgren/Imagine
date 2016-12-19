#pragma once

#include <functional>
#include <base.hpp>

#include "interface/element.hpp"
#include "interface/controls/icons.hpp"
#include "interface/images.hpp"
#include "settings/settings.hpp"


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