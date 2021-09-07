#ifndef STRINGBLOB_H
#define STRINGBLOB_H

#include <sstream>
#include <string>
#include "../include/Formatter.h"


class StringBlob: public Formatter {
    private:
        std::string message;
    public:
        StringBlob() : message("") {}
        StringBlob(std::string _msg) : message(_msg) {}

        std::string FormatToString() {
            return message;
        }
};

#endif