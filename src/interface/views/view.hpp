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
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;

            ScreenArea contentBox;
    };

}