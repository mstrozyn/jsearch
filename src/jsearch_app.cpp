#include <iostream>

#include "utils.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "too few program arguments" << std::endl;
        std::cerr << "usage: 'jsearch /search/path server_address:server:port'" << std::endl;
        return EXIT_FAILURE;
    }

    // search for files with the given extensions
    std::list<std::string> files;
    search_for_files(argv[1], ".json", files);
    search_for_files(argv[1], ".json-bak", files);

    if (files.empty()) {
        std::cout << "no files found" << std::endl;
        return EXIT_SUCCESS;
    }

    // search files for the modules versions
    nlohmann::json result;
    search_for_versions(files, result);

    if (result.empty()) {
        std::cout << "no versions found" << std::endl;
        return EXIT_SUCCESS;
    }

    std::string server_address(argv[2]);
    std::string port = server_address.substr(server_address.find(":") + 1);
    std::string address = server_address.substr(0, server_address.find(":"));

    // send found modules versions to the server
    if (send_to_server(address, port, result.dump()) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
