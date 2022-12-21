/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : app/menus/comp/button.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 18 Dec 2022
 EDITED : 21 Dec 2022
 ******************************************************************************/

/******************************************************************************
 An interactive button that can call a function when interacted with
 ******************************************************************************/

#pragma once

#include "../../../core.hpp"

// Used to store cursor position and button positions
struct point
{
    uint32_t x;
    uint32_t y;
};

class Button
{
public:

    // Constructors
    Button();
    Button(point _position,
           std::string _inactive_display,
           std::string _active_display,
           bool _enabled,
           std::function<void()> _on_click
          );

    // Destructor
    virtual ~Button();

    // Performs the function defined by on_click.
    void click();

    // Sets the display's active state
    void setActive(bool state);
    // Changes the button's position
    void setPosition(point new_position);
    // Gets the button's position
    point getPosition();
    // Enables/Disables the button
    void setEnabled(bool state);
    // Returns if the button is enabled
    bool isEnabled();
    // Sets the inactive display string
    void setInactiveDisplay(std::string new_display);
    // Sets the active display string
    void setActiveDisplay(std::string new_display);
    // Gets the current display
    std::string getCurrentDisplay();
    // Sets the on_click function
    void setOnClick(std::function<void()> _on_click);

private:
    // Position to render the button
    point position;

    // If the button is clickable
    bool enabled;

    // Strings to used when inactive and active
    std::string inactive_display;
    std::string active_display;

    // String to be printed
    std::string current_display;

    // Function to call when button is activated
    std::function<void()> on_click;

};