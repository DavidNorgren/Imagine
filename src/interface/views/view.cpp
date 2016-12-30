#include "interface/views/view.hpp"
#include "imagineapp.hpp"


void Imagine::View::update()
{
    contentBox = {
        { pos.x, pos.y + VIEW_HEADER_HEIGHT },
        box.width,
        box.height - VIEW_HEADER_HEIGHT
    };
}


void Imagine::View::draw()
{
    drawBox(box, SETTING_INTERFACE_COLOR_BACKGROUND);
    drawBox(contentBox, { 0.3f });
}


void Imagine::View::mouseEvent()
{

}


void Imagine::View::keyEvent()
{

}