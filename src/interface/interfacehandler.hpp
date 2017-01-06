#pragma once

#include <vector>
#include <functional>
#include <base.hpp>

#include "interface/controls/control.hpp"
#include "interface/controls/button.hpp"
#include "interface/controls/checkbox.hpp"
#include "interface/controls/row.hpp"
#include "interface/controls/scrollbar.hpp"
#include "interface/controls/textfield.hpp"
#include "interface/controls/labeledgroup.hpp"
#include "interface/tabs/tabcollection.hpp"
#include "interface/views/view.hpp"
#include "interface/images.hpp"
#include "interface/interfacestate.hpp"
#include "settings/settings.hpp"


namespace Imagine
{
    
    /* Stores and manipulates the components of the interface. */
    class InterfaceHandler
    {
        public:
            /* Initializes the interface. */
            InterfaceHandler();

            /* Update methods. */
            void update();
            void draw();
            void mouseEvent();
            void keyEvent();
            
            /* The current action by the user */
            InterfaceState state = IDLE;

            /* The currently focused element in the interface. */
            Element* focus = nullptr;
            
            /* Root container of the interface. */
            Container* rootContainer;

            /* Create the tabs. */
            Tab* createTabProperties();
            Tab* createTabTimeline();
            Tab* createTabSettings();

            /* When moving a container/tab. */
            Container* moveContainer;
            Container::InsertPosition moveContainerInsertPosition;
            int moveIndex;
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