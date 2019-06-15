#include "io.hpp"

void io::submitError(std::string text)
{
	cout_error << "[ERROR] " << text << "\n";
}

void io::submitMessage(std::string text)
{
	cout_message << text << "\n";
}

void io::flushError()
{
	std::cout << cout_error.str();
	std::cout.flush();
	cout_error.clear();
}

void io::flushMessage()
{
	std::cout << cout_message.str();
	std::cout.flush();
	cout_message.clear();
}