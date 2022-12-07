/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : main.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 7 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Handles loading standard libraries and utils for other source files
 ******************************************************************************/

#pragma once

// Standard Library //
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>

// Windows Libraries //
#ifdef _WIN32

#include <Windows.h>
#include <shlobj.h>

#endif

// External Libraries //
#include <fmt/core.h>

// Utils //
#include "util/emath.hpp"
#include "util/logger.hpp"
