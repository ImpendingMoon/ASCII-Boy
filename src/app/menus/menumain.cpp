/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : app/menus/menumain.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 18 Dec 2022
 EDITED : 21 Dec 2022
 ******************************************************************************/

/******************************************************************************
 The main starting menu for the program. Allows the user to load a ROM,
 go to options, or exit.
 ******************************************************************************/

#include "menumain.hpp"

// Constructor
MenuMain::MenuMain()
{
    // Start position for buttons
    point buttonPos = {(GB_X_RES / 2) - 7, 30};

    createButton(
        buttonPos,
        "[x] Play Game",
        "[ ] Play Game",
        true,
        nullptr
    );

    createButton(
        {buttonPos.x, buttonPos.y + 2},
        "[x] Load ROM",
        "[ ] Load ROM",
        true,
        nullptr
    );

    createButton(
        {buttonPos.x, buttonPos.y + 4},
        "[x] Settings",
        "[ ] Settings",
        true,
        nullptr
    );

    createButton(
        {buttonPos.x, buttonPos.y + 6},
        "[x] Exit",
        "[ ] Exit",
        true,
        [](){ setState(EXITING); }
    );
}

// Destructor
MenuMain::~MenuMain() = default;


// Initializes and renders the menu
void MenuMain::initMenu()
{
    renderMenu();
}

// Renders a menu
void MenuMain::renderMenu()
{
    // Render the main logo
    renderLogo();

    // Print border around window
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');

    // Print buttons
    for(Button button : buttons)
    {
        point pos = button.getPosition();

        // Move cursor to position
        wmove(stdscr, pos.y, pos.x);

        // Print display text
        printw(button.getCurrentDisplay().c_str());

    }
}

// Renders the logo specified in the array above
void MenuMain::renderLogo()
{
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
}