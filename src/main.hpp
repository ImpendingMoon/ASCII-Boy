/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : main.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 11 Dec 2022
 ******************************************************************************/

/******************************************************************************
 The main entrypoint for ASCII-Boy.
 ******************************************************************************/

#pragma once

#include "core.hpp"
#include "app/program.hpp"

// Safely exits the program when an exit signal is called
void exitHandler(int signal);