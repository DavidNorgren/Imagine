#pragma once

#include "interface/element.hpp"


namespace Imagine
{

    /* Represents a container with resizable and movable child elements. */
    class Container : public Element
    {
        public:
            enum Layout
            {
                HORIZONTAL,
                VERTICAL,
                OTHER
            };
            enum SizeMode
            {
                FIXED,
                FILL
            };

            Container(Layout layout, std::vector<Container*> subContainers, SizeMode sizeMode = FILL)
            {
                this->layout = layout;
                this->subContainers = subContainers;
                this->sizeMode = sizeMode;
            }
                
            /* Element methods. */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            void setParent(Element* parent) override;
            
            Layout layout;
            std::vector<Container*> subContainers;

            SizeMode sizeMode;
            bool visible = true;
            int sizeFixed = 0, sizeVisible, level;
            float sizeFill = 1.f;
    };

}