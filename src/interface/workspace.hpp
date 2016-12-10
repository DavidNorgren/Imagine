#pragma once

#include "interface/controls/control.hpp"
#include "interface/controls/button.hpp"
#include "interface/controls/checkbox.hpp"
#include "interface/controls/row.hpp"
#include "interface/controls/textbox.hpp"
#include "interface/controls/labeledgroup.hpp"
#include "interface/panels/panel.hpp"

#define PANEL_START_WIDTH   380
#define PANEL_START_HEIGHT  210
#define PANEL_MIN_WIDTH     300
#define PANEL_MIN_HEIGHT    50


namespace Mineimator
{

    class Workspace : public Element
    {
        public:
            
            Workspace();
            
            /* Element methods. */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            void setParent(Element* parent) override;

            /* Create the tabs. */
            void initTabProperties();
            void initTabTimeline();
            void initTabSettings();

            std::vector<Panel*> panels;
            Tab* tabProperties;
            Tab* tabTimeline;
            Tab* tabSettings;
    };

}