#pragma once

#include "interface/container.hpp"

namespace Imagine
{

    /* A view into the 3D scene. */
    class View : public Container
    {
        public:
            View() :
                Container(OTHER, {}, FILL) {};

            /* Element methods. */
            void update();
            void draw();
            void mouseEvent();
            void keyEvent();

            ScreenArea contentBox;
    };

}