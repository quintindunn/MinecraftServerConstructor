#include "cfg.h"

#include "minecraftvanilla.h"

#include "util.h"
#include "requester.h"
#include "javadownloader.h"

#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <format>
#include <filesystem>

#include "minecraft.h"

using json = nlohmann::json;

typedef struct MinecraftVersion MinecraftVersion;

namespace Vanilla {
	std::string get_jar(MinecraftVersion version, std::string output_dir, bool download_java = false) {
		json version_manifest = json::parse(http_get(version.url, REQUEST_RETRY_COUNT));

		std::string server_jar_url = version_manifest["downloads"]["server"]["url"];
		std::string server_jar_sha1 = version_manifest["downloads"]["server"]["sha1"];
		
		if (!CreateDirectoryIfNotExist(output_dir)) {
			std::cerr << "Error creating " << output_dir << std::endl;
			return "";
		}
#ifdef VANILLA_JAR_DEBUG
		std::cout << output_dir + "/server.jar" << std::endl;
#endif
		http_get_file(server_jar_url, output_dir + "/server.jar", REQUEST_RETRY_COUNT);

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
		MinecraftVersion version = Minecraft::get_version(options.id);
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

		sign_eula(options.server_path + "/eula.txt");
		write_start_script(options);
		cleanup();
		return true;
	}

	void cleanup() {
		std::error_code errorCode;
		if (!std::filesystem::remove_all(TMP_DIR, errorCode)) {
			std::cout << errorCode.message() << std::endl;
		}
	}

}
