#ifndef FORMATTER_H
#define FORMATTER_H

#include <string>


class Formatter {
    public:
        virtual std::string FormatToString() = 0;
};

#endif