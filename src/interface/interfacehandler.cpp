#include "interface/interfacehandler.hpp"
#include "mineimatorapp.hpp"


Mineimator::InterfaceHandler::InterfaceHandler()
{
    workspace = new Workspace();
    
    // This is the root
    setParent(nullptr);
}


void Mineimator::InterfaceHandler::update()
{
    box = { { 0, 0 }, app->mainWindow->width, app->mainWindow->height };
    workspace->box = box;
    workspace->update();
}


void Mineimator::InterfaceHandler::draw()
{
    drawBegin();
    
    // Dummy background
    drawBox(box, { 0.1f });
    
    workspace->draw();
}


void Mineimator::InterfaceHandler::mouseEvent()
{
    if (app->interfaceHandler->state == IDLE && mouseLeftPressed()) {
        focus = nullptr;
    }
    workspace->mouseEvent();
}


void Mineimator::InterfaceHandler::keyEvent()
{
    workspace->keyEvent();
}


void Mineimator::InterfaceHandler::setParent(Element* parent)
{
    this->parent = parent;
    workspace->setParent(this);
}


void Mineimator::setInterfaceState(InterfaceState state)
{
    app->interfaceHandler->state = state;
}


Mineimator::InterfaceState Mineimator::getInterfaceState()
{
    return app->interfaceHandler->state;
}