#ifndef IO_H
#define IO_H

#include <sstream>
#include <string>
#include <iostream>
#include <ostream>

#define STATUS_SUCCESS 0
#define STATUS_FAILED 1
#define STATUS_FAILEDLOAD 2
#define STATUS_FAILEDWRITE 3

class io
{
private:
public:
	std::ostringstream cout_error;
	std::ostringstream cout_message;

	void submitError(std::string text);
	void submitMessage(std::string text);

	void flushError();
	void flushMessage();
};

#endif