#include "LogRecord.h"

LogRecord::LogRecord() {}
LogRecord::LogRecord(std::vector<char>&& msg): mMessage(msg) {}

std::vector<char> LogRecord::getMessage() {
    return std::move(mMessage);
}

char* LogRecord::getMessageData(uint64_t size) {
    mMessage.resize(size, 0);
    return mMessage.data();
}


StringRecord::StringRecord(const std::string& str)
: LogRecord(std::vector<char>(str.begin(), str.end())) {}