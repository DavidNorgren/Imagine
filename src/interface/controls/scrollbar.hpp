#pragma once

#include "interface/controls/control.hpp"


namespace Mineimator
{

    class ScrollBar : public Element
    {
        public:
            /* Scrollbar direction. */
            enum Direction
            {
                VERTICAL,
                HORIZONTAL
            };

            ScrollBar(Direction direction, int* offset)
            {
                this->direction = direction;
                this->offset = offset;
            }

            /* Element methods. */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;

            Direction direction;
            ScreenArea bar;
            int visibleSize, totalSize, *offset, clickOffset;
            bool visible, pressed = false;
    };

}