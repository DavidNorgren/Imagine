#pragma once

#include "interface/element.hpp"

namespace Imagine
{

    /* A view into the 3D scene. */
    class View : public Element
    {
        public:
            /* Element methods. */
            void update();
            void draw();
            void mouseEvent();
            void keyEvent();

            ScreenArea contentBox;
    };

}