#include "HttpHandler.h"
#include "Log.h"

#include <regex>
#include <iomanip>

const std::string DownloadFilePath = "File\\Download\\";

int CHttpHandler::check_end_boundary(const std::string& buffer, const std::string& end_boundary) {
	auto endPos = buffer.rfind(end_boundary);
	if (endPos != buffer.npos) {
		LOG_INFO("<HttpHandler> File end boundary matched: {}", end_boundary.c_str());
		return endPos;
	}
	//else {
	//	LOG_WARN("<HttpHandler> file end boundary not found.");
	//}
    return -1;
}

std::string CHttpHandler::getHeaderValue(const std::string& buffer, const std::string& filter) {
    std::smatch match;
    std::string value;
    if (std::regex_search(buffer, match, std::regex(filter))) {
        value = match[1];
        LOG_DEBUG("<HttpHandler> Extracted header: {}", match[0].str().c_str());
    }
    return value;
}

std::array<std::string, 4> CHttpHandler::extractHeaders(const std::string& buffer) {
    std::string filename;
    std::string fileChecksum;
    std::smatch match;
    std::string content;

    std::string file_boundary = getHeaderValue(buffer, R"(--([^\r\n]+))");
    if (file_boundary.size() == 0) {
        LOG_INFO("<HttpHandler> file boundary is not found");
        return { };
    }

    //filename
    filename = getHeaderValue(buffer, R"(Content-Disposition: form-data; name="file"; filename="([^"]+))");
    if (filename.size()) {
        LOG_INFO("<HttpHandler> Extracted filename: {}", filename.c_str());
        
        //File Checksum
        fileChecksum = getHeaderValue(buffer, R"(File-Checksum:\s*([a-fA-F0-9]{64}))");
        if (fileChecksum.size()) {
            LOG_INFO("<HttpHandler> Extracted file Checksum: {}", fileChecksum.c_str());
        }

        auto startPos = buffer.find(fileChecksum);
        if (startPos != buffer.npos) {
            content = buffer.substr(startPos + fileChecksum.length() + std::string("\r\n\r\n").length());
        }
    }
    else {
        LOG_INFO("<HttpHandler> Filename or its header is not found in the request.");
    }

    return { filename, fileChecksum, "--" + file_boundary + "--", content };
}

void CHttpHandler::handleFileWrite(std::string& buffer, Client::DataMap* dataMap) {
    if (!dataMap)
        return;

    std::ofstream& outfile = dataMap->outfile;
    if (dataMap->file_write_inProgress && outfile.is_open()) {
        auto endPos = check_end_boundary(buffer, dataMap->file_boundary);
        if (endPos >= 0) {
            buffer = buffer.substr(0, endPos - 2);
		}
        if (buffer.size()) {
            outfile.write(buffer.c_str(), buffer.length());
            dataMap->filesize += buffer.length();
            SHA256_Update(&(dataMap->sha256_ctx), (void*)buffer.data(), buffer.length());
        }
	}
    else if (!dataMap->file_write_inProgress) {
        auto data = extractHeaders(buffer);
        dataMap->filename = data[0];
        dataMap->fileChecksum = data[1];
        dataMap->file_boundary = data[2];
        if (!dataMap->filename.empty()) {
            dataMap->file_write_inProgress = true;
            const std::string filepath = DownloadFilePath + dataMap->filename;
            outfile.open(filepath, std::ios::binary);
            if (!outfile.is_open()) {
                LOG_ERROR("<HttpHandler> Failed to open file for writing: {}", filepath.c_str());
                return;
            }
            SHA256_Init(&(dataMap->sha256_ctx));
            LOG_INFO("<HttpHandler> start downloding file: {}", filepath.c_str());
            buffer = data[3];
            handleFileWrite(buffer, dataMap);
        }
        else {
            LOG_ERROR("<HttpHandler> Header form-data, Filename not extracted, or request does not contain needed fields.");
            return;
        }
    }
}

bool CHttpHandler::validateFileChecksum(Client::DataMap* dataMap) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &(dataMap->sha256_ctx));
    std::ostringstream hex_stream;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        hex_stream << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    std::string fileChecksumSha256 = hex_stream.str();
    if (fileChecksumSha256 != dataMap->fileChecksum) {
        LOG_WARN("Http downloaded file checksum mismatch");
        return false;
    }
    return true;
}