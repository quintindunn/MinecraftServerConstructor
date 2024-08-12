#include <format>
#include <iostream>
#include <string>
#include <array>
#include <memory>

#include <filesystem>

#include "minecraftspigot.h"
#include "minecraft.h"

#include "javadownloader.h"

#include "cfg.h"
#include "util.h"
#include "requester.h"

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif


std::string exec(std::string cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

namespace Spigot {
	void download_buildtools(SpigotOptions options) {
		if (!CreateDirectoryIfNotExist(TMP_DIR)) {
			std::cerr << "Error creating " << TMP_DIR << " directory." << std::endl;
			return;
		}
		options.buildtools_path = std::format("{}/BuildTools.jar", TMP_DIR);
		http_get_file(SPIGOT_BUILDTOOLS_URL, options.buildtools_path, REQUEST_RETRY_COUNT);
	}

	void build_spigot(SpigotOptions options) {
		std::filesystem::path owd = std::filesystem::current_path();

		std::filesystem::current_path(TMP_DIR);
		std::string command = std::format("call \".{}bin/java.exe\" -Xmx{}M -Xms{}M -jar BuildTools.jar --rev {}", BUILDTOOLS_MAX_RAM, BUILDTOOLS_MAX_RAM, options.min_allocated_mb, options.id);
		//std::string result = exec(command);
		std::filesystem::current_path(owd);

		std::cout << owd << std::endl;
		std::cout << std::format("{}/spigot-{}.jar", TMP_DIR, options.id) << std::endl;
		std::cout << std::format("../{}/server.jar", options.server_path) << std::endl;

		std::filesystem::rename(std::format("{}/spigot-{}.jar", TMP_DIR, options.id), std::format("{}/server.jar", options.server_path));
	}

	bool fullSetup(SpigotOptions options) {
		if (!CreateDirectoryIfNotExist(TMP_DIR)) {
			std::cerr << "Error making " << TMP_DIR << " directory." << std::endl;
			return false;
		}
		if (!CreateDirectoryIfNotExist(JAVA_ROOT_DIR)) {
			std::cerr << "Error making " << JAVA_ROOT_DIR << " directory." << std::endl;
			return false;
		}
		if (!CreateDirectoryIfNotExist(options.server_path)) {
			std::cerr << "Error making " << options.server_path << " directory." << std::endl;
			return false;
		}

		download_buildtools(options);

		MinecraftVersion version = Minecraft::get_version(options.id);
		std::string java_version = Minecraft::get_java_version(version);
		
		options.java_executable_path = download_jre(java_version);

		build_spigot(options);
		return true;
	}
};