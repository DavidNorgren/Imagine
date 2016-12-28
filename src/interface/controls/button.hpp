#pragma once

#include "interface/controls/control.hpp"


namespace Imagine
{

    /* A button with an icon and text. */
    class Button : public Control
    {
        public:
            Button(string name,
                   std::function<void()> clickFunc = nullptr,
                   int icon = NO_ICON,
                   int height = 30,
                   std::function<bool()> visibleFunc = TRUE_FUNC)
            {
                this->name = name;
                this->clickFunc = clickFunc;
                this->icon = icon;
                this->height = height;
                this->visibleFunc = visibleFunc;
            };

            /* Element methods. */
            void update() override;
            void draw() override;
            
        protected:
            int icon, height;
    };
    
    
    /* A button with an icon only. */
    class IconButton : public Button
    {
        public:
            IconButton(string name,
                       bool showText = true,
                       std::function<void()> clickFunc = nullptr,
                       int icon = NO_ICON,
                       int width = 16,
                       int height = 16,
                       std::function<bool()> visibleFunc = TRUE_FUNC,
                       std::function<int()> iconFunc = nullptr) :
                Button(name,
                       clickFunc,
                       icon,
                       height,
                       visibleFunc
                )
            {
                this->showText = showText;
                this->width = width;
                this->iconFunc = iconFunc;
            }
                  
            /* Element methods. */
            void update() override;
            void draw() override;
            
        private:
            bool showText;
            int width;
            std::function<int(void)> iconFunc;
    };
    
}