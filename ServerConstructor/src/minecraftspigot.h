#pragma once

struct SpigotOptions {
	std::string id;
	std::string server_path;
	std::string start_script_name;
	size_t max_allocated_mb;
	size_t min_allocated_mb;
	std::string java_executable_path = "";
	std::string buildtools_path = "";
};

namespace Spigot {
	void download_buildtools(SpigotOptions);

	bool fullSetup(SpigotOptions);
}