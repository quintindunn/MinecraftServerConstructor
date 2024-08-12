#pragma once
#include <string>

bool CreateDirectoryIfNotExist(std::string path);
std::string unzip(const char* zipFilename, const char* extractPath);
