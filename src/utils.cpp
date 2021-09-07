#include <iostream>
#include <sstream>
#include <cstdio>
#include <chrono>
#include <ctime>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>
#include <algorithm>
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


std::string randomString(uint64_t length) {
    auto randchar = []() -> char {
        const char charset[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(length,0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}