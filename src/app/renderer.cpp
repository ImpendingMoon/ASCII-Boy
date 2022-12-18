/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : app/renderer.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 17 Dec 2022
 EDITED : 17 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Handles all rendering via ncurses
 ******************************************************************************/

#include "renderer.hpp"

constexpr uint64_t GB_X_RES = 160;
constexpr uint64_t GB_Y_RES = 144;

// Initializes the renderer
void initRenderer()
{
    // Init the main screen, resize to Gameboy's resolution
    initscr();
    resize_term(GB_Y_RES, GB_X_RES);
    wresize(stdscr, GB_Y_RES, GB_X_RES);
    // Enter RAW mode
    cbreak();
    noecho();
    // Handle special keys (backspace, shift, etc.)
    keypad(stdscr, true);
}



// Renders to the screen
void render()
{
    wrefresh(stdscr);
}



// Exits and cleans up ncurses
void exitRenderer()
{
    endwin();
}