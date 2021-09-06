#ifndef LOGITERATOR_H
#define LOGITERATOR_H

#include "LogWriter.h"
#include <cstdint>
#include <vector>
#include <memory>

class LogIterator {
    private:
        LogWriter* pLogWriter;
        uint64_t currentIndex;
    
    public:
        LogIterator(LogWriter* _pLogWriter);
        LogIterator(const LogIterator& other);
        

        // overload operators
        bool operator==(const LogIterator& it);
        bool operator!=(const LogIterator& it);
        void operator++(int);
        void operator++();
        LogRecord operator*();
        LogIterator operator+(uint64_t offset) const;


        LogRecord getCurrentRecord();
        uint64_t getCurrentRecordIndex() const;
        void LogIteratorBegin();
        void LogIteratorEnd();
};

#endif