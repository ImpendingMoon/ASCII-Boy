/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : main.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 11 Dec 2022
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
#include <thread>
#include <array>
#include <cstdint>
#include <climits>
#include <vector>

// Windows Libraries //
#ifdef _WIN32

#include <Windows.h>
#include <shlobj.h>

#endif

// External Libraries //
#define FMT_HEADER_ONLY
#include "../lib/fmt/core.h"

// Utils //
#include "util/emath.hpp"
#include "util/logger.hpp"
