#pragma once
#include <string>

#include "cfg.h"
#define UA "ServerConstructor/0.1"

std::string http_get(std::string);
std::string http_get(std::string, int);
void http_get_file(std::string, std::string);
void http_get_file(std::string, std::string, int);