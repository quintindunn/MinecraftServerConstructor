#pragma once
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "minecraft.h"

using json = nlohmann::json;

struct VanillaOptions {
	std::string id;
	std::string server_path;
	std::string start_script_name;
	size_t max_allocated_mb;
	size_t min_allocated_mb;
	std::string java_executable_path = "";
};

namespace Vanilla {
	std::string get_jar(MinecraftVersion, std::string, bool);
	bool sign_eula(std::string);
	bool write_start_script(VanillaOptions);
	bool fullSetup(VanillaOptions options);
	void cleanup(void);
};

