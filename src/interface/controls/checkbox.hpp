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
                this->clickFunc = [this]() {
                    this->checked = !this->checked;
                    if (this->checkFunc) {
                        this->checkFunc();
                    }
                };
            };

            /* Element methods. */
            void update() override;
            void draw() override;

        private:
            std::function<void()> checkFunc;
            bool checked = false; // Todo: function to get check state
    };
    
}