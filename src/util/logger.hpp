/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : util/logger.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 5 Dec 2022
 EDITED : 18 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Handles logging to the console and logfile
 ******************************************************************************/

#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include "../../lib/fmt/core.h"

namespace Logger
{
	// LogLevel is used to set what logs will be output from the settings
	// If a log()'s level is above the setting's log level, it will not be sent
	enum LogLevel
	{			  // Settings            | Log()
		NONE = 0, // Log nothing         | Message will not be logged
		ERRORS,   // Log program errors  | Use for errors in program, not emu
		VERBOSE,  // Log verbose info    | Use for program information
		DEBUG,    // Log emu debug info  | Use for errors in emulator
		EXTREME,  // Log everything      | Use for early debugging
		// Extreme *WILL* slow down the program and create GBs worth of logs
		// The MMU, CPU, and PPU use it to log every single operation they do.
	};

	// Initializes the Logger and Logfile
	void initLogger();
	// Closes the Logfile
	void exitLogger();

	// Gets the log level
	int getLogLevel();
	// Sets the log level
	void setLogLevel(LogLevel newLevel);
	// Gets if logging to console
	bool isLogToConsole();
	// Sets if logging to console
	void setLogToConsole(bool value);
	// Gets if logging to file
	bool isLogToFile();
	// Sets if logging to file
	void setLogToFile(bool value);
	// Gets the current file path for the LogFile
	std::string getLogFilePath();
	// Returns if the LogFile is open
	bool isLogFileOpen();

	// Logs a message with the given level
	void log(std::string message, LogLevel level);
	// Logs a message with a default level (Verbose)
	void log(std::string message);
};
