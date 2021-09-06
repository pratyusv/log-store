#ifndef LOGRECORD_H
#define LOGRECORD_H

#include <vector>
#include <string>
#include <memory>


class LogRecord {
    private:
        std::vector<char> mMessage;
    
    public:
        LogRecord();
        LogRecord(std::vector<char> &&msg);
        std::vector<char> getMessage();
        char* getMessageData(uint64_t size);
};


class StringRecord: public LogRecord {
    public:
        StringRecord(const std::string& str);
};

#endif