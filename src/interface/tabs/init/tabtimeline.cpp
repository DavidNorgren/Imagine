#include "interface/interfacehandler.hpp"


Imagine::Tab* Imagine::InterfaceHandler::createTabTimeline()
{
    return
        new Tab("Timeline", true, false, {
            new TabSection("Section A", true, {
                new CheckBox("Test")
            })
        });
}