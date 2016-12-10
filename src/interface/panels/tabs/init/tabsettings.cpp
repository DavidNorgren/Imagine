#include "interface/interfacehandler.hpp"


void Mineimator::Workspace::initTabSettings()
{
    tabSettings = new Tab("Settings", true, false,
        {
            new TabSection("Section A", true,
                {
                    new CheckBox("Test")
                }
            )
        }
    );
}