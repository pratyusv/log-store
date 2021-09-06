
#include <cstdint>

const uint64_t BYTES = 1024;
const uint64_t MEGABYTE = BYTES*BYTES;

// Max Record Size 32 MB
const uint64_t MAX_RECORD_SIZE = 32 * MEGABYTE;
const std::string LOG_EXTENSION = "data";
const uint64_t MAX_LEN_FILENAME = 1024;