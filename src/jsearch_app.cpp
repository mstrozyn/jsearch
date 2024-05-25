#include <list>
#include <iostream>
#include <filesystem>

#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// searches for files with the given extension on the specified path and adds to the list
void search_for_files(const std::string& path, const std::string& extension, std::list<std::string>& files) {
    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(path)) {
        if (!dir_entry.path().extension().compare(extension)) {
            files.push_back(dir_entry.path());
        }
    }
}

// sends the given message to the specified address and port
int send_to_server(const std::string& address, const std::string& port, const std::string& message) {
    if (address.empty() || port.empty()) {
        std::cerr << "wrong server address:port" << std::endl;
        return -1;
    }

    if (message.empty()) {
        std::cerr << "no server message to send" << std::endl;
        return -1;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(std::stoul(port));
    server.sin_addr.s_addr = inet_addr(address.c_str());

    int client = socket(AF_INET, SOCK_DGRAM, 0);
    if (client < 0 ) {
        std::cerr << "socket creation failed" << std::endl;
        return -1;
    }

    int result = sendto(client, (const char *)message.c_str(), strlen(message.c_str()), 0, (const struct sockaddr *)&server, sizeof(server));
    if (result < 0 ) {
        std::cerr << "sending to server failed" << std::endl;
    }

    close(client);
    return result;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "too few program arguments" << std::endl;
        std::cerr << "usage: 'jsearch /search/path server_address:server:port'" << std::endl;
        return EXIT_FAILURE;
    }

    std::list<std::string> files;
    search_for_files(argv[1], ".json", files);
    search_for_files(argv[1], ".json-bak", files);
    for (auto const& file: files) {
        std::cout << file << std::endl;
    }

    std::string result = "";
    //std::string result = "{\"module1\":\"v1.0\", \"module2\":\"v2.2\", \"module3\":\"v3.1\"}";

    std::string server_address(argv[2]);
    std::string port = server_address.substr(server_address.find(":") + 1);
    std::string address = server_address.substr(0, server_address.find(":"));

    if (send_to_server(address, port, result) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
