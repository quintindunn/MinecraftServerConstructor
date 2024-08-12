#include <iostream>
#include "minecraftvanilla.h"
#include "minecraftspigot.h"
#include "minecraft.h"

std::string convertToLowercase(const std::string& str)
{
	std::string result = "";

	for (char ch : str) {
		result += tolower(ch);
	}

	return result;
}

void vanilla() {
	VanillaOptions server_options;
	std::cout << "Server Version: ";
	std::string id;
	std::cin >> id;

	server_options.id = id;
	server_options.max_allocated_mb = 1024;
	server_options.min_allocated_mb = 1024;
	server_options.server_path = "./server";
	server_options.start_script_name = "run.cmd";
	Vanilla::fullSetup(server_options);
}

void spigot() {
	SpigotOptions server_options;
	std::cout << "Server Version: ";
	std::string id;
	std::cin >> id;

	server_options.id = id;
	server_options.max_allocated_mb = 1024;
	server_options.min_allocated_mb = 1024;
	server_options.server_path = "./server";
	server_options.start_script_name = "run.cmd";

	Spigot::fullSetup(server_options);
}

int main() {
	std::string architecture;
	std::cout << "vanilla/spigot: ";
	std::cin >> architecture;

	architecture = convertToLowercase(architecture);

	if (architecture == "spigot") {
		spigot();
	}
	else if (architecture == "vanilla") {
		vanilla();
	}
	else {
		std::cout << "Architecture \"" << architecture << "\" not found!";
	}

	return 0;
}