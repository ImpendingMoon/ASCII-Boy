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

#pragma once

#include "../core.hpp"
// NOTE: Untested on Windows. May need to use PDCurses.
#include <ncurses.h>

// Initializes the renderer
void initRenderer();

// Renders to the screen
void render();

// Exits and cleans up ncurses
void exitRenderer();