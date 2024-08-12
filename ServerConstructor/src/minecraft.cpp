#include <nlohmann/json.hpp>

#include "requester.h"
#include "minecraft.h"

using json = nlohmann::json;

namespace Minecraft {
	json get_versions_manifest() {
		static std::string manifest_raw = http_get(MINECRAFT_VERSION_MANIFEST, REQUEST_RETRY_COUNT);
		static json manifest = json::parse(manifest_raw);
		return manifest;
	}

	std::vector<MinecraftVersion> get_versions() {
		json manifest_versions = get_versions_manifest()["versions"];

		std::vector<MinecraftVersion> versions;

		for (json json_version : manifest_versions) {
			MinecraftVersion version{};

			version.id = json_version["id"];
			version.url = json_version["url"];
			version.time = json_version["time"];
			version.releaseTime = json_version["releaseTime"];
			version.sha1 = json_version["sha1"];

			std::string type = json_version["type"];
			if (type == "release") { version.type = VersionType::release; }
			else if (type == "snapshot") { version.type = VersionType::snapshot; }
			else if (type == "old_beta") { version.type = VersionType::old_beta; }
			else if (type == "old_alpha") { version.type = VersionType::old_alpha; }
			else { version.type = VersionType::UNKNOWN; }

			versions.push_back(version);
		}

		return versions;
	}

	MinecraftVersion get_version(std::string id) {
		for (MinecraftVersion version : get_versions()) {
			if (version.id == id) {
				return version;
			}
		}
	}

	MinecraftVersion get_latest(std::string type) {
		json manifest = get_versions_manifest();
		std::string id = manifest["latest"][type];
		return get_version(id);
	}
};