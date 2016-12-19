#pragma once

#include <vector>
#include <functional>
#include <base.hpp>

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

            /* Initializes the interface. */
            InterfaceHandler();

            /* Element methods. */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            void setParent(Element* parent) override;
            
            /* Area consisting of up to six visible panels and
               a view space with two visible views. */
            Workspace* workspace;
            
            /* The current action by the user */
            InterfaceState state = IDLE;

            /* The currently focused element in the interface. */
            Element* focus = nullptr;
    };

    /* Get/set interface state. */
    void setInterfaceState(InterfaceState state);
    InterfaceState getInterfaceState();
    bool isInterfaceState(InterfaceState state);

    /* Get/set focused element. */
    void setFocused(Element* element);
    Element* getFocused();
    bool isFocused(Element* element);
}