#ifndef ERRORBLOB_H
#define ERRORBLOB_H

#include <sstream>
#include <string>
#include "../include/Formatter.h"



class ErrorBlob: public Formatter {
    private:
        uint64_t eventId;
        std::string description;
        uint64_t processId;
        std::string process;
    public:
        ErrorBlob(uint64_t _evId, std::string _desc, uint64_t _pId, std::string _process) 
            : eventId(_evId), description(_desc), processId(_pId), process(_process)  {}

        std::string FormatToString() {
            std::stringstream ss;
            ss << "Event : " << eventId << "\n";
            ss << "Process: [ Id:" << processId << " Name: " << process << "] \n";
            ss << "Description: " << description;
            return ss.str();
        }
};



#endif