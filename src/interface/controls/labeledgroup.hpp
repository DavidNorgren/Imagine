#pragma once

#include "interface/controls/control.hpp"


namespace Mineimator
{
    
    class LabeledGroup : public Control
    {
        public:
            LabeledGroup(std::vector<Control*> controls) {
                this->controls = controls;
            }

            /* Element methods. */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            void setParent(Element* parent) override;
            
        private:
            std::vector<Control*> controls;
    };
    
}