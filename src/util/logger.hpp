/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : util/logger.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 5 Dec 2022
 EDITED : 7 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Handles logging to the console and logfile
 ******************************************************************************/

#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <fmt/core.h>

// Logger is a singleton that handles writing to console/logfile
// NOTE: Creation is not thread-safe, but is called in main() before anything
// else is started, so it should be fine for this program.
class Logger
{
private:
	// Private constructor/destructor
	Logger();
	virtual ~Logger();

	// Gets a timestamp. Used in log()
	std::string getTimestamp();

	std::string log_file_path;
	std::ofstream LogFile;

	int log_level;
	bool log_to_console;
	bool log_to_file;

public:
	// Gets the instanced Logger object
	static Logger& instance()
	{
		static Logger instance;
		return instance;
	}

	// Singleton stuff
	Logger(Logger const&) = delete;
	void operator = (Logger const&) = delete;


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

	// Actually useful functions //

	// Gets the current file path for the LogFile
	std::string getLogFilePath();

	// Returns if the LogFile is open
	bool isLogFileOpen();

	// Logs a message with the given level
	void log(std::string message, LogLevel level);

	// Logs a message with a default level (Verbose
	void log(std::string message);
};
