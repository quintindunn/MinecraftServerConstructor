#include <string>
#include <format>
#include <iostream>

#include <nlohmann/json.hpp>

#include "cfg.h"
#include "util.h"
#include "requester.h"


using json = nlohmann::json;

std::string download_jre(std::string majorVersion) {
	std::string url = std::format(JAVA_MANIFEST_FORMAT, majorVersion);

	if (!CreateDirectoryIfNotExist(TMP_DIR)) {
		fprintf(stderr, "Error making %s directory", TMP_DIR);
		return "";
	}

	json manifest_root = json::parse(http_get(url, REQUEST_RETRY_COUNT));

	std::string target = "";
	for (json manifest : manifest_root) {
		for (json binary : manifest["binaries"]) {
			if (binary["os"] == JAVA_OS && binary["image_type"] == "jre" && binary["architecture"] == JAVA_ARCHITECTURE) {
				target = binary["package"]["link"];
#ifdef JAVA_DOWNLOADER_DEBUG
				std::cout << "Found Java version link " << target << std::endl;
#endif
				break;
			}
		}
		if (target != "")
			break;
	}


	if (target == "") {
		std::cerr << "Couldn't find java version!" << std::endl;
		return "";
	}
	std::string java_zip = std::format("{}/java.zip", TMP_DIR);
	http_get_file(target, java_zip, 3);
	#ifdef JAVA_DOWNLOADER_DEBUG
		std::cout << "Got java.zip, located at " << java_zip << std::endl;
	#endif
	std::string extracted_dir = JAVA_ROOT_DIR + (std::string)"/" + unzip(java_zip.c_str(), "./java/");

	if (extracted_dir == "") {
		std::cerr << "Error extracting java.zip" << std::endl;
		return "";
	}
	return extracted_dir;
}
