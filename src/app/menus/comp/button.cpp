/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : app/menus/comp/button.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 18 Dec 2022
 EDITED : 21 Dec 2022
 ******************************************************************************/

/******************************************************************************
 An interactive button that can call a function when interacted with
 ******************************************************************************/

#include "button.hpp"

// Constructors
Button::Button()
{
    position = {0, 0};
    inactive_display = "[ ] Button";
    active_display = "[x] Button";
    enabled = true;
    on_click = nullptr;
    current_display = inactive_display;
}


Button::Button(
        point _position,
        std::string _inactive_display,
        std::string _active_display,
        bool _enabled,
        std::function<void()> _on_click
        )
{
    position = _position;
    inactive_display = _inactive_display;
    active_display = _active_display;
    on_click = _on_click;
    enabled = _enabled;

    current_display = inactive_display;
}


// Destructor
Button::~Button() = default;




// Performs the function defined by on_click.
void Button::click()
{
    if(enabled && on_click != nullptr)
    {
        on_click();
    }
}




// Sets the display's active state
void Button::setActive(bool state)
{
    current_display = state ? active_display : inactive_display;
}



// Gets the current display
std::string Button::getCurrentDisplay()
{
    return current_display;
}



// Changes the button's position
void Button::setPosition(point new_position)
{
    position = new_position;
}





// Gets the button's position
point Button::getPosition()
{
    return position;
}



void Button::setEnabled(bool state)
{
    enabled = state;
}



bool Button::isEnabled()
{
    return enabled;
}

// Sets the inactive display string
void Button::setInactiveDisplay(std::string new_display)
{
    inactive_display = new_display;
}



// Sets the active display string
void Button::setActiveDisplay(std::string new_display)
{
    active_display = new_display;
}



// Sets the on_click function
void Button::setOnClick(std::function<void()> _on_click)
{
    on_click = _on_click;
}