#include "interface/views/view.hpp"
#include "imagineapp.hpp"


void Imagine::View::update()
{
    std::cout << stringRepeat("\t", level) << "view " << std::endl;
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
    
    drawBox(insertLeftBox, Color(COLOR_YELLOW, 0.25f));
    drawText("left", insertLeftBox.pos, COLOR_WHITE);
    drawBox(insertRightBox, Color(COLOR_BLUE, 0.25f));
    drawText("right", insertRightBox.pos, COLOR_WHITE);
    drawBox(insertTopBox, Color(COLOR_GREEN, 0.25f));
    drawText("top", insertTopBox.pos, COLOR_WHITE);
    drawBox(insertBottomBox, Color(COLOR_RED, 0.25f));
    drawText("bottom", insertBottomBox.pos, COLOR_WHITE);
}


void Imagine::View::mouseEvent()
{

}


void Imagine::View::keyEvent()
{

}