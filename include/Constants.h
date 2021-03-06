#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>
#include <string>

#define LOGGER_PRETTY_TIME_FORMAT "[%Y-%m-%d %H:%M:%S] "


const uint64_t BYTES = 1024;
const uint64_t MEGABYTE = BYTES*BYTES;

// Max Record Size 32 MB
const uint64_t MAX_RECORD_SIZE = 32 * MEGABYTE;
const std::string LOG_EXTENSION = "log";
const std::string CONFIG_EXTENSION = "config";
const uint64_t MAX_LEN_FILENAME = 1024;
const std::string TEMP_FILE_NAME = "tmp_";


#endif