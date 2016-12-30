#include "interface/container.hpp"
#include <iostream>

void Imagine::Container::update()
{
    // Calculate visible size of each subcontainer
    int totalSize = 0;

    // Fixed size (toolbar and tab containers)
    for (Container* container : subContainers) {
        if (container->sizeMode == FIXED && container->visible) {
            container->sizeVisible = container->sizeFixed;
            totalSize += container->sizeVisible;
        }
    }

    // Filled size (views and other containers)
    for (Container* container : subContainers) {
        if (container->sizeMode == FILL && container->visible) {
            container->sizeVisible = ((layout == HORIZONTAL ? box.width : box.height) - totalSize) * container->sizeFill;
        }
    }

    // Define boxes
    ScreenPos currentPos = pos;
    for (Container* container : subContainers)
    {
        if (container->visible)
        {
            container->pos = currentPos;
            if (layout == HORIZONTAL)
            {
                container->box.width = container->sizeVisible;
                container->box.height = box.height;
                container->update();
                currentPos.x += container->sizeVisible;
            }
            else
            {
                container->box.width = box.width;
                container->box.height = container->sizeVisible;
                container->update();
                currentPos.y += container->sizeVisible;
            }
        }
    }
}

void Imagine::Container::draw()
{
    for (Container* container : subContainers)
    {
        if (container->visible) {
            container->draw();
        }
    }
}


void Imagine::Container::mouseEvent()
{
    for (Container* container : subContainers)
    {
        if (container->visible) {
            container->mouseEvent();
        }
    }
}


void Imagine::Container::keyEvent()
{
    for (Container* container : subContainers)
    {
        if (container->visible) {
            container->keyEvent();
        }
    }
}

void Imagine::Container::setParent(Element* parent)
{
    this->parent = parent;
    for (Container* container : subContainers) {
        container->setParent(this);
    }
}