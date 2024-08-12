#include "cfg.h"

#include "minecraftvanilla.h"
#include "requester.h"
#include "util.h"
#include "javadownloader.h"

#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <format>

using json = nlohmann::json;

typedef struct VanillaVersion VanillaVersion;

namespace Vanilla {
	json get_versions_manifest() {
		static std::string manifest_raw = http_get(MINECRAFT_VERSION_MANIFEST);
		static json manifest = json::parse(manifest_raw);
		return manifest;
	}

	std::vector<VanillaVersion> get_versions() {
		json manifest_versions = get_versions_manifest()["versions"];
		
		std::vector<VanillaVersion> versions;

		for (json json_version : manifest_versions) {
			VanillaVersion version{};

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

	VanillaVersion get_version(std::string id) {
		for (VanillaVersion version : get_versions()) {
			if (version.id == id) {
				return version;
			}
		}
	}

	VanillaVersion get_latest(std::string type) {
		json manifest = get_versions_manifest();
		std::string id = manifest["latest"][type];
		return get_version(id);
	}

	std::string get_jar(VanillaVersion version, std::string output_dir, bool download_java = false) {
		json version_manifest = json::parse(http_get(version.url));

		std::string server_jar_url = version_manifest["downloads"]["server"]["url"];
		std::string server_jar_sha1 = version_manifest["downloads"]["server"]["sha1"];
		
		if (!CreateDirectoryIfNotExist(output_dir)) {
			std::cerr << "Error creating " << output_dir << std::endl;
			return "";
		}
#ifdef VANILLA_JAR_DEBUG
		std::cout << output_dir + "/server.jar" << std::endl;
#endif
		http_get_file(server_jar_url, output_dir + "/server.jar");

		int javaMajorVersion = version_manifest["javaVersion"]["majorVersion"];
		return std::to_string(javaMajorVersion);
	}

	bool sign_eula(std::string eula_path) {
		std::ofstream eula(eula_path);
		if (eula.is_open()) {
			eula << EULA_SIGNATURE;
			eula.close();
			return true;
		}

		std::cerr << "Unable to open " << eula_path << std::endl;
		return false;
	}

	bool write_start_script(VanillaOptions options) {
		std::string script_path = std::format("{}/{}", options.server_path, options.start_script_name);

		std::ofstream script(script_path);
		if (script.is_open()) {
			script << std::format("\".{}\" -Xmx{}M -Xms{}M -jar server.jar nogui", options.java_executable_path, options.max_allocated_mb, options.min_allocated_mb);
			script.close();
			return true;
		}

		std::cerr << "Unable to open " << script_path << std::endl;
		return false;
	}

	bool fullSetup(VanillaOptions options) {
		VanillaVersion version = Vanilla::get_version(options.id);
		std::string java_major_version = Vanilla::get_jar(version, options.server_path, false);
#ifdef FULLSETUP_DEBUG
		std::cout << "Java Major Version " << java_major_version << std::endl;
#endif
		std::string java_executable = download_jre(java_major_version) + "bin/java.exe";
		if (java_executable == ".bin/java.exe") {
			std::cerr << "Error downloading java..." << std::endl;
			return false;
		}
		options.java_executable_path = java_executable;

		Vanilla::sign_eula(options.server_path + "/eula.txt");
		Vanilla::write_start_script(options);
		return true;
	}

}
