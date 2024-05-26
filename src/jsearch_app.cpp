#include <iostream>

#include "utils.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "too few program arguments" << std::endl;
        std::cerr << "usage: 'jsearch /search/path server_address:server_port'" << std::endl;
        return EXIT_FAILURE;
    }

    // search for files with the given extensions
    std::list<std::string> files;
    std::string search_path(argv[1]);
    if (utils::search_for_files(search_path, ".json", files) < 0) {
        std::cerr << "searching for .json files failed" << std::endl;
        return EXIT_FAILURE;
    }
    if (utils::search_for_files(search_path, ".json-bak", files) < 0) {
        std::cerr << "searching for .json-bak files failed" << std::endl;
        return EXIT_FAILURE;
    }
    if (files.empty()) {
        std::cout << "no files found" << std::endl;
        return EXIT_SUCCESS;
    }

    // search files for the modules versions
    nlohmann::json result;
    utils::search_for_versions(files, result);
    if (result.empty()) {
        std::cout << "no versions found" << std::endl;
        return EXIT_SUCCESS;
    }

    // send found modules versions to the server
    std::string server_address(argv[2]);
    std::string port = server_address.substr(server_address.find(":") + 1);
    std::string address = server_address.substr(0, server_address.find(":"));
    if (utils::send_to_server(address, port, result.dump()) < 0) {
        std::cerr << "sending to server failed" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
