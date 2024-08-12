#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

enum VersionType {
	release,
	snapshot,
	old_beta,
	old_alpha,
	UNKNOWN,
};

struct MinecraftVersion {
	std::string id;
	VersionType type;
	std::string url;
	std::string time;
	std::string releaseTime;
	std::string sha1;
	std::string javaMajorVersion = "";
};

namespace Minecraft {
	json get_versions_manifest();
	std::vector<MinecraftVersion> get_versions();
	MinecraftVersion get_version(std::string);
	MinecraftVersion get_latest(std::string);
	std::string get_java_version(MinecraftVersion version);
};