#ifndef UTILS_H
#define UTILS_H

#include <string>

uint64_t GetFreeSpace();
std::string GetFileName(const std::string& fileName, const std::string& extension, uint64_t index = 0);
bool  RenameFileName(const std::string &oldFileName, const std::string &newFileName);
uint64_t GetFileSize(const std::string &fileName, const uint64_t &index);
bool DeleteFile(const std::string &fileName, const uint64_t &index);
std::string IntToString(uint64_t value);
uint64_t StringToInt(std::string& str);
std::string GetCurrentTimeStamp();

#endif