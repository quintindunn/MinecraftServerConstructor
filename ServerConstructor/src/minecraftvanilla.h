#pragma once
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum VersionType {
	release,
	snapshot,
	old_beta,
	old_alpha,
	UNKNOWN,
};

struct VanillaVersion {
	std::string id;
	VersionType type;
	std::string url;
	std::string time;
	std::string releaseTime;
	std::string sha1;
};

struct VanillaOptions {
	std::string id;
	std::string server_path;
	std::string start_script_name;
	size_t max_allocated_mb;
	size_t min_allocated_mb;
	std::string java_executable_path = "";
};

namespace Vanilla {
	json get_versions_manifest();
	std::vector<VanillaVersion> get_versions();
	VanillaVersion get_version(std::string);
	VanillaVersion get_latest(std::string);
	std::string get_jar(VanillaVersion, std::string, bool);
	bool sign_eula(std::string);
	bool write_start_script(VanillaOptions);
	bool fullSetup(VanillaOptions options);
	void cleanup(void);
};

