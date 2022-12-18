/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : app/menus/menumain.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 18 Dec 2022
 EDITED : 18 Dec 2022
 ******************************************************************************/

/******************************************************************************
 The main starting menu for the program. Allows the user to load a ROM,
 go to options, or exit.
 ******************************************************************************/

#include "menumain.hpp"

// Constructor
MenuMain::MenuMain() = default;

// Destructor
MenuMain::~MenuMain() = default;


// Initializes and renders the menu
void MenuMain::initMenu()
{
    // Render the main logo
    // Line # to start printing logo
    constexpr uint32_t Y_OFFSET = 10;

    for(uint32_t i = 0; i < logo.size(); i++)
    {
        std::string line = logo.at(i);

        // Calculate center position
        uint64_t center = (GB_X_RES / 2) - (line.length() / 2);

        // Print line at correct position
        mvprintw(i + Y_OFFSET, center, "%s", line.c_str());
    }

    // Print border around window
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');

    render();
}


// Exits the menu
void MenuMain::exitMenu()
{

}


// Goes to another menu
void MenuMain::gotoMenu(Menu& menu)
{

}


// Moves the cursor to the next valid position
void MenuMain::moveCursor()
{

}


// Puts the terminal into cooked move and gets a string from the user
std::string MenuMain::getString()
{

}