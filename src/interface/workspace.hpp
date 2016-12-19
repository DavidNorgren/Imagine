#pragma once

#include "interface/controls/control.hpp"
#include "interface/controls/button.hpp"
#include "interface/controls/checkbox.hpp"
#include "interface/controls/row.hpp"
#include "interface/controls/textbox.hpp"
#include "interface/controls/labeledgroup.hpp"
#include "interface/panels/panel.hpp"
#include "interface/views/view.hpp"


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

            /* Panels */
            std::vector<Panel*> panels;
            Tab* tabProperties;
            Tab* tabTimeline;
            Tab* tabSettings;

            /* Views */
            ScreenArea viewArea;
            View* viewMain;
            View* viewSecond;
    };

}