#include <list>
#include <string>

#include "json.hpp"

/**
 * searches for files
 *
 * @param[in] path path to check
 * @param[in] extension file extension to check
 * @param[out] files resulting list of files
 */
void search_for_files(const std::string& path, const std::string& extension, std::list<std::string>& files);

/**
 * searches for modules versions
 *
 * @param[in] files file list to check
 * @param[out] result resulting json file with versions
 */
void search_for_versions(const std::list<std::string>& files, nlohmann::json& result);

/**
 * sends the message to the server
 *
 * @param[in] address server address
 * @param[in] port server port
 * @param[in] message message to send
 * @return non negative value on success
 */
int send_to_server(const std::string& address, const std::string& port, const std::string& message);
