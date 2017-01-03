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
}


void Imagine::InterfaceHandler::mouseEvent()
{
    if (isInterfaceState(IDLE) && mouseLeftPressed()) {
        focus = nullptr;
    }
    rootContainer->mouseEvent();
    
    if (isInterfaceState(TAB_MOVE)) {
        getFocused()->mouseEvent();
    }
}


void Imagine::InterfaceHandler::keyEvent()
{
    rootContainer->keyEvent();
}


void Imagine::setInterfaceState(InterfaceState state)
{
    app->interfaceHandler->state = state;
}


Imagine::InterfaceState Imagine::getInterfaceState()
{
    return app->interfaceHandler->state;
}


bool Imagine::isInterfaceState(InterfaceState state)
{
    return getInterfaceState() == state;
}


void Imagine::setFocused(Element* element)
{
    app->interfaceHandler->focus = element;
}


Imagine::Element* Imagine::getFocused()
{
    return app->interfaceHandler->focus;
}


bool Imagine::isFocused(Element* element)
{
    return getFocused() == element;
}