#ifndef UTILS_H
#define UTILS_H

#include <string>

uint64_t GetFreeSpace();
std::string GetTempFileName();
std::string GetFileName(const std::string& fileName, const std::string& extension, uint64_t index);
bool  RenameFileName(const std::string &oldFileName, const std::string &newFileName);
uint64_t GetFileSize(const std::string &fileName, const uint64_t &index);
bool DeleteFile(const std::string &fileName, const uint64_t &index);

#endif