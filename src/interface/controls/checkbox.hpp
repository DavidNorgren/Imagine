#pragma once

#include "interface/controls/control.hpp"


namespace Mineimator
{
    
    /* A checkbox with an on/off state. */
    class CheckBox : public Control
    {
        public:

            CheckBox(string name,
                     std::function<void()> checkFunc = nullptr,
                     std::function<bool()> visibleFunc = TRUE_FUNC)
            {
                this->name = name;
                this->checkFunc = checkFunc;
                this->visibleFunc = visibleFunc;
                height = 16;
                checked = false;
            };

            /* Element methods. */
            void update() override;
            void draw() override;
            //void mouseEvent(MouseEvent* event) override;
            //void keyEvent(KeyEvent* event) override;

            std::function<void()> checkFunc;
            bool checked; // Todo: function to get check state
    };
    
}