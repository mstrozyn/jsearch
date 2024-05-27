#include <fstream>
#include <iostream>

#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utils.h"

namespace utils {

static void update_version(const nlohmann::json& module, nlohmann::json& result) {
    const std::string MODULE_NAME("module_name");
    const std::string FEATURE_VERSION("feature_version");

    if (module.contains(MODULE_NAME) && (module.contains(FEATURE_VERSION))) {
        std::string current_version("v0");
        const std::string new_version(module[FEATURE_VERSION]);

        if (result.contains(module[MODULE_NAME])) {
            current_version = result[module[MODULE_NAME]];
        }

        if (std::stof(new_version.substr(1)) > std::stof(current_version.substr(1))) {
            // record module and version
            result[module[MODULE_NAME]] = new_version;
        }
    }
}

void search_for_versions(const std::list<std::string>& files, nlohmann::json& result) {
    const std::string MODULES("modules");

    for (auto const& file: files) {
        try {
            std::ifstream json_file(file);
            nlohmann::json data = nlohmann::json::parse(json_file);

            if (!data.contains(MODULES)) {
                continue;
            }

            auto const& modules = data[MODULES];
            if (!modules.is_array()) {
                continue;
            }

            for (nlohmann::json::const_iterator it = modules.begin(); it != modules.end(); ++it) {
                update_version(*it, result);
            }
        } catch (...) {
            // do nothing, just move on to the next item
        }
    }
}

int search_for_files(const std::string& path, const std::string& extension, std::list<std::string>& files) {
    if (path.empty() || !std::filesystem::exists(path)) {
        std::cerr << "wrong path: " << path << std::endl;
        return -1;
    }

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(path)) {
        if (!dir_entry.path().extension().compare(extension)) {
            files.push_back(dir_entry.path());
        }
    }

    return 0;
}

int send_to_server(const std::string& address, const std::string& port, const std::string& message) {
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;

    if (address.empty() || (inet_aton(address.c_str(), &server.sin_addr) == 0)) {
        std::cerr << "wrong address: " << address << std::endl;
        return -1;
    }

    try {
        unsigned long port_number = std::stoul(port);
        if (port_number > 65535) {
            std::cerr << "port number out of range: " << port_number << std::endl;
            return -1;
        }
        server.sin_port = htons(port_number);
    } catch (...) {
        std::cerr << "wrong port: " << port << std::endl;
        return -1;
    }

    int client = socket(AF_INET, SOCK_DGRAM, 0);
    if (client < 0 ) {
        std::cerr << "socket creation failed" << std::endl;
        return -1;
    }
    int result = sendto(client, message.c_str(), strlen(message.c_str()),
                        0, (const struct sockaddr *)&server, sizeof(server));

    close(client);

    return result;
}

} // utils
