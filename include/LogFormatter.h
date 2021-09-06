#ifndef LOGFORMATTER_H
#define LOGFORMATTER_H

#include "LogRecord.h"
#include <string>


class LogFormatter {
    public:
        static std::string toString(LogRecord&& record) {
            std::vector<char> vec = record.getMessage();
            return std::string(vec.begin(), vec.end());
        }
};


#endif