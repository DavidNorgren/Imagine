#include <iostream>

#include "mineimatorapp.hpp"


int main()
{ 
    new Mineimator::App();
    return 0;
}


Mineimator::App* app;

Mineimator::App::App()
{
    app = this;
    interfaceHandler = new InterfaceHandler();
    mainWindow->setTitle("Mine-imator");
    launch();
}


void Mineimator::App::loopEvent()
{
    interfaceHandler->draw();
}


void Mineimator::App::mouseEvent(MouseEvent* event)
{
    //interfaceHandler->mouseEvent(event);
}


void Mineimator::App::keyEvent(KeyEvent* event)
{
    //interfaceHandler->keyEvent(event);
}


void Mineimator::App::resizeEvent()
{
    interfaceHandler->update();
}