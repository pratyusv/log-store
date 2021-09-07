#ifndef INTBLOB_H
#define INTBLOB_H

#include <sstream>
#include <string>
#include "../include/Formatter.h"

class IntBlob: public Formatter {
    private:
        uint64_t number;
    public:
        IntBlob() : number(0) {}
        IntBlob(uint64_t _num) : number(_num) {}

        std::string FormatToString() {
            std::stringstream ss;
            ss << number;
            return ss.str();
        }
};



#endif