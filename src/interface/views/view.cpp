#include "interface/views/view.hpp"
#include "imagineapp.hpp"


void Imagine::View::update()
{
    contentBox = {
        { pos.x, pos.y + VIEW_HEADER_HEIGHT },
        box.width,
        box.height - VIEW_HEADER_HEIGHT
    };

    // Define boxes for inserting
    insertBox[LEFT] = {
        pos + (ScreenPos){ insertOffset[LEFT], insertOffset[TOP] },
        box.width / 3 - insertOffset[LEFT],
        box.height - (insertOffset[TOP] + insertOffset[BOTTOM])
    };
    insertBox[RIGHT] = {
        { pos.x + (box.width / 3) * 2, pos.y + insertOffset[TOP] },
        box.width / 3 - insertOffset[RIGHT],
        box.height - (insertOffset[TOP] + insertOffset[BOTTOM])
    };
    insertBox[TOP] = {
        { pos.x + box.width / 3, pos.y + insertOffset[TOP] },
        box.width / 3,
        box.height / 2 - insertOffset[TOP]
    };
    insertBox[BOTTOM] = {
        { pos.x + box.width / 3, pos.y + box.height / 2 },
        box.width / 3,
        box.height / 2 - insertOffset[BOTTOM]
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