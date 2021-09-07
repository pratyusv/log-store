#ifndef LOGITERATOR_H
#define LOGITERATOR_H

#include "LogWriter.h"
#include <cstdint>
#include <vector>
#include <memory>


template<typename T>
class LogIterator {
private:
    LogWriter<T>* pLogWriter;
    uint64_t currentIndex;

public:
    LogIterator(LogWriter<T>* _pLogWriter) 
        : pLogWriter(_pLogWriter), currentIndex(0) {}

    LogIterator(const LogIterator<T>& other) {
        pLogWriter = other.pLogWriter;
        currentIndex = other.currentIndex;
    }

    

    // overload operators
    bool operator!=(const LogIterator& it) {
        return this->getCurrentRecordIndex() != it.getCurrentRecordIndex();
    }

    std::string operator*() {
        return this->getCurrentRecord();
    }

    void operator++(int) {
        currentIndex++;
    }

    void operator++() {
        currentIndex++;
    }

    LogIterator<T> operator+(uint64_t offset) const {
        LogIterator it(*this);
        it.LogIteratorBegin();
        it.currentIndex += offset;
        return it;
    }

    bool operator==(const LogIterator& it) {
        return this->getCurrentRecordIndex() == it.getCurrentRecordIndex();
    }


    uint64_t getCurrentRecordIndex() const {
        return currentIndex;
    }

    std::string getCurrentRecord() {
        std::string record;
        if (!pLogWriter->ReadRecord(currentIndex, record)) {
            currentIndex = pLogWriter->GetEndRecordIndex() - 1;
        }
        return record;
    }

    void LogIteratorBegin() {
        currentIndex = pLogWriter->GetStartRecordIndex();
    }

    void LogIteratorEnd() {
        currentIndex = pLogWriter->GetEndRecordIndex();
    }

};

#endif