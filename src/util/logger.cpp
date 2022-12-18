/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : util/logger.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 5 Dec 2022
 EDITED : 18 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Handles logging to the console and logfile
 ******************************************************************************/

#include "logger.hpp"

// Gets a timestamp. Used in log()
std::string getTimestamp();


std::string log_file_path;
std::ofstream LogFile;

int log_level;
bool log_to_console;
bool log_to_file;

// Constructor
void Logger::initLogger()
{
	// The LogFile is placed in either %APPDATA%\ImpendingMoon\ASCII-Boy
	// or ~/.local/share/ASCII-Boy/
#ifdef _WIN32
	log_file_path = getenv("APPDATA");
	log_file_path.append("\\ImpendingMoon\\ASCII-Boy\\");

#else
	log_file_path = getenv("HOME");
	log_file_path.append("/.local/share/ASCII-Boy/");

#endif

	try {
		// Check to see if directory exists. If not, create it.
		if(!std::filesystem::exists(log_file_path))
		{
			std::filesystem::create_directories(log_file_path);
		}
	} catch(std::filesystem::filesystem_error& ex) {

		std::cerr << "ERROR: Cannot create logfile directory.\n";
		std::cerr << "Log File Path: " << log_file_path << "\n";
		std::cerr << ex.what();

		// Exit Code 74 - IOERROR - from sysexits.h
		exit(74);
	}

	log_file_path.append("asciiboy.log");

	// Open/Create the LogFile for writing
	LogFile.open(log_file_path, std::ios_base::out);

	// Check to see if the file is valid. If not, exit.
	if(!LogFile)
	{
		std::cerr << "ERROR: Cannot access logfile.\n";
		std::cerr << "Log File Path: " << log_file_path << "\n";

		exit(74);
	}

	// LogFile should now be opened and writable.

	// TODO: Load these from a config
	log_level = EXTREME;
	log_to_console = false; // Best not to use cout with curses
	log_to_file = true;
}


// Closes the Logfile
void Logger::exitLogger()
{
	// Close LogFile before destroying self
	LogFile.close();
}



// Gets the log level
int Logger::getLogLevel()
{
	return log_level;
}


// Sets the log level
void Logger::setLogLevel(LogLevel newLevel)
{
	log_level = newLevel;
}


// Gets if logging to console
bool Logger::isLogToConsole()
{
	return log_to_console;
}


// Sets if logging to console
void Logger::setLogToConsole(bool value)
{
	log_to_console = value;
}


// Gets if logging to file
bool Logger::isLogToFile()
{
	return log_to_file;
}


// Sets if logging to file
void Logger::setLogToFile(bool value)
{
	log_to_file = value;
}



// Gets the current file path for the LogFile
std::string Logger::getLogFilePath()
{
	return log_file_path;
}



// Returns if the LogFile is open
bool Logger::isLogFileOpen()
{
	return LogFile.is_open();
}



// Logs a message with a default level (verbose)
void Logger::log(std::string message)
{
	log(message, VERBOSE);
}



// Logs a message with the given level
void Logger::log(std::string message, LogLevel level)
{
	if(level <= log_level)
	{
		if(log_to_console)
		{
			std::cout << "[" << getTimestamp() << "] ";
			std::cout << message << "\n";
		}
		if(log_to_file)
		{
			LogFile << "[" << getTimestamp() << "] ";
			LogFile << message << "\n";
		}
	}
}



// Gets a timestamp. Used in log()
std::string getTimestamp()
{
	// Get time and convert it to local time
	time_t now = std::time(0);
	tm *ltime = localtime(&now);

	// Format local time as a string
	std::string output = fmt::format("{:02d}:{:02d}:{:02d}",
					ltime->tm_hour, ltime->tm_min, ltime->tm_sec);

	return output;
}
