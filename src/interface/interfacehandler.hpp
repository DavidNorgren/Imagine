#pragma once

#include <vector>
#include <functional>

#include "base.hpp"
using namespace Base;

#include "interface/interfacestate.hpp"
#include "interface/workspace.hpp"
#include "interface/images.hpp"
#include "settings/settings.hpp"


namespace Mineimator
{
    
    /* Stores and manipulates the components of the interface. */
    class InterfaceHandler : public Element
    {
        public:

            /* Initialize interface. */
            InterfaceHandler();

            /* Element methods */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            void setParent(Element* parent) override;
            
            Workspace* workspace;
            
            InterfaceState state = IDLE;
            Control* focus = nullptr;
    };

}