#include "interface/interfacehandler.hpp"


void Mineimator::Workspace::initTabTimeline()
{
    tabTimeline = new Tab("Timeline", true, false,
        {
            new TabSection("Section A", true,
                {
                    new CheckBox("Test")
                }
            )
        }
    );
}