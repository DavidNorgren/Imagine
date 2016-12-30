#pragma once

#include "interface/panels/panel.hpp"
#include "interface/views/view.hpp"


namespace Imagine
{

    class Workspace : public Element
    {
        public:
            enum PanelLocation
            {
                LEFT_BOTTOM,
                RIGHT_BOTTOM,
                BOTTOM,
                TOP,
                LEFT_TOP,
                RIGHT_TOP
            };
            
            Workspace();
            
            /* Element methods. */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            void setParent(Element* parent) override;

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