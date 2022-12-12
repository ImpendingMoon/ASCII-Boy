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
#include "emu/gbsystem.hpp"

// Safely exits the program when an exit signal is called
void exitHandler(int signal);

enum ProgramState
{
    STOPPED, // Emulator is not running - at user prompts
    RUNNING, // Emulator is running
    PAUSED,  // Emulator is paused
    EXITING, // Program is preparing to exit
};

ProgramState programState = STOPPED;