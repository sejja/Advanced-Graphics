#include "Logger.h"
#include <fstream>
#include <ctime>

using namespace Core;
using namespace std;

const string FILENAME = "info.log";

Logger::Logger() {
#if _DEBUG
	levelStrings[LogLevel::ERROR] = "ERROR";
	levelStrings[LogLevel::WARNING] = "WARNING";
	levelStrings[LogLevel::INFO] = "INFO";

	file.open(FILENAME);
	//file << "Logger stardddddted" << endl;
	logMessage("Logger is set up");
	//file.close();
#endif
}

Logger::~Logger() {
#if _DEBUG
	file.close();
	//cout << "cerrar" << endl;
#endif
}

void Logger::logMessage(LogLevel level, string message) {
	time_t now = time(0);
	tm* localTime = localtime(&now);
	
	char timeText[30];
	sprintf(timeText, "%d-%d-%d %d:%d:%d ", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
	string text = timeText + levelStrings[level] + " " + message;
	cout << text << endl;
	file << text << endl;
}

void Logger::logMessage(string message) {
	this->logMessage(LogLevel::INFO, message);
}