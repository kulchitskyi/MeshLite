#pragma once
#include <string>

class FileDialog final
{
private:

public:
	static std::string openFile(const char* filter);
};