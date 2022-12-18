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

constexpr uint64_t GB_X_RES = 160;
constexpr uint64_t GB_Y_RES = 144;

// Initializes the renderer
void initRender();

// Exits the renderer
void exitRenderer();