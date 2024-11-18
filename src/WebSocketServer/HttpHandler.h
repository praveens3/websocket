#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include "utils.h"

#include <string>
#include <array>

class CHttpHandler {
public:
    static int check_end_boundary(const std::string& buffer, const std::string& file_boundary);
    static std::string getHeaderValue(const std::string& buffer, const std::string& filter);
    static std::array<std::string, 4> extractHeaders(const std::string& buffer);
    static void handleFileWrite(std::string& buffer, Client::DataMap* dataMap);
    static bool validateFileChecksum(Client::DataMap* dataMap);
};

#endif // HTTPHANDLER_H
