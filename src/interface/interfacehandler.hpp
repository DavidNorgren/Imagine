#pragma once

#include <vector>
#include <functional>

#include "base.hpp"
using namespace Base;

#include "interface/controls/control.hpp"
#include "interface/controls/button.hpp"
#include "interface/controls/checkbox.hpp"
#include "interface/controls/row.hpp"
#include "interface/controls/textbox.hpp"
#include "interface/panels/panel.hpp"
#include "interface/images.hpp"
#include "settings/settings.hpp"

#define PANEL_START_WIDTH   380
#define PANEL_START_HEIGHT  210
#define PANEL_MIN_WIDTH     300
#define PANEL_MIN_HEIGHT    50


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
            
            /* Create the tabs. */
            void createTabProperties();
            void createTabTimeline();
            
            std::vector<Panel*> panels;
            Tab* tabProperties;
            Tab* tabTimeline;
    };

}