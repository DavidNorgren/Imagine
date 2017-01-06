#include "imagineapp.hpp"


Imagine::InterfaceHandler::InterfaceHandler()
{
    // Set up initial container structure
    rootContainer =
        new Container(Container::HORIZONTAL, {
            new Container(Container::VERTICAL, {
                new Container(Container::HORIZONTAL, {
                    new View(),
                    new TabCollection({
                        createTabSettings()
                    })
                }),
                new TabCollection({
                    createTabTimeline()
                })
            }),
            new TabCollection({
                createTabProperties()
            })
        });

    // Set root
    rootContainer->setParent(nullptr);
}


void Imagine::InterfaceHandler::update()
{
    rootContainer->box = { { 0, 0 }, app->mainWindow->width, app->mainWindow->height };
    rootContainer->update();
}


void Imagine::InterfaceHandler::draw()
{
    drawBegin();
    rootContainer->draw();

    if (isInterfaceState(TAB_MOVE)) {
        getFocused()->draw();
    }

    drawTextAligned(toString(app->mainWindow->fps), { app->mainWindow->width, 0 }, RIGHT, TOP, COLOR_YELLOW);
}


void Imagine::InterfaceHandler::mouseEvent()
{
    if (isInterfaceState(IDLE) && mouseLeftPressed()) {
        setFocused(nullptr);
    }

    if (isInterfaceState(TAB_MOVE))
    {
        moveContainer = nullptr;
        rootContainer->mouseEvent();
        getFocused()->mouseEvent();

        // Release the tab
        if (!mouseLeftDown())
        {
            // Mouse is in a blank spot somewhere
            if (!moveContainer) {
                moveContainer = rootContainer;
                moveContainerInsertPosition = Container::RIGHT;
                moveIndex = moveContainer->subContainers.size() - 1;
            }

            moveContainer->addTab((Tab*)getFocused(), moveContainerInsertPosition, moveIndex);
            setInterfaceState(IDLE);
            moveContainer = nullptr;
        }
    }
    else {
        rootContainer->mouseEvent();
    }
}


void Imagine::InterfaceHandler::keyEvent()
{
    rootContainer->keyEvent();
}


void Imagine::setInterfaceState(InterfaceState state)
{
    interface->state = state;
}


Imagine::InterfaceState Imagine::getInterfaceState()
{
    return interface->state;
}


bool Imagine::isInterfaceState(InterfaceState state)
{
    return getInterfaceState() == state;
}


void Imagine::setFocused(Element* element)
{
    interface->focus = element;
}


Imagine::Element* Imagine::getFocused()
{
    return interface->focus;
}


bool Imagine::isFocused(Element* element)
{
    return getFocused() == element;
}