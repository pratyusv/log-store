#include <iostream>
#include <sstream>
#include <cstdio>
#include <chrono>
#include <ctime>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>
#include "utils.h"
#include "Constants.h"


uint64_t GetFreeSpace() {

    struct statvfs stat;
    struct passwd *pw = getpwuid(getuid());

    if (pw != NULL && statvfs(pw->pw_dir, &stat) == 0) {
        uint64_t freeBytes = static_cast<uint64_t>(stat.f_bavail * stat.f_frsize);
        return freeBytes;
    }
    return 0;
}

std::string GetFileName(const std::string& fileName, const std::string& extension, uint64_t index)
{
    std::stringstream ss;
    ss << fileName;

    if (index > 0) {
        ss << '_' << index;
    }

    if (!extension.empty()) {
        ss << '.' << extension;
    }

    return ss.str();
}


bool  RenameFileName(const std::string &oldFileName, const std::string &newFileName)
{
    int res = rename(oldFileName.c_str(), newFileName.c_str());
    if (res != 0) {
        return false;
    }
    return true; 
}


uint64_t GetFileSize(const std::string &fileName, const uint64_t &index)
{
    struct stat filestatus;
    std::string fileNameWithExtension = GetFileName(fileName, LOG_EXTENSION, index);
    stat(fileNameWithExtension.c_str(), &filestatus);
    return filestatus.st_size;
}

bool DeleteFile(const std::string &fileName, const uint64_t &index) {
    std::string fileNameWithExtension = GetFileName(fileName, LOG_EXTENSION, index);
    int retval = unlink(fileNameWithExtension.c_str());
    if (retval != 0) {
        return false;
    }
    return true;
}


std::string IntToString(uint64_t value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}


uint64_t StringToInt(std::string &str) {
    std::stringstream ss(str);
    uint64_t i;
    ss >> i;
    return i;
}


// format it in two parts: main part with date and time and part with milliseconds
std::string GetCurrentTimeStamp()
{
    auto tp = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(tp);

    // this function use static global pointer. so it is not thread safe solution
    std::tm* time_info = std::localtime(&current_time);

    char buffer[128];

    int string_size = strftime(
        buffer, sizeof(buffer),
        LOGGER_PRETTY_TIME_FORMAT,
        time_info
    );

    return std::string(buffer, buffer + string_size);
}
