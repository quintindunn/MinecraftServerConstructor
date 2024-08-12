#include <iostream>
#include "minecraftvanilla.h"


int main() {
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

	return 0;
}