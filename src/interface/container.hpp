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
                insertLeftSize = insertRightSize = insertTopSize = insertBottomSize = 0;
            }

            /* Add a subcontainer */
            void addSubContainer(Container* container, int index = -1);

            /* Remove a subcontainer. */
            void removeSubContainer(Container* container);

            /* Replaces a subcontainer. */
            void replaceSubContainer(Container* oldContainer, Container* newContainer);
                
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
            int sizeFixed = CONTAINER_START_SIZE, level = 0;
            float sizeFill = 1.f;

            ScreenArea insertLeftBox, insertRightBox, insertTopBox, insertBottomBox;
            int insertLeftSize, insertRightSize, insertTopSize, insertBottomSize;
    };

}