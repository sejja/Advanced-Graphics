#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <fstream>
#include <map>

#undef ERROR;


enum LogLevel {
	INFO,
	WARNING,
	ERROR
};

namespace Core {



class Logger {
	std::map<LogLevel, std::string> levelStrings;
	public:
		Logger();
		~Logger();
		void logMessage(LogLevel level, std::string message);
		void logMessage(std::string message);

	private:
		std::ofstream file;
		

	
};


}
#endif
