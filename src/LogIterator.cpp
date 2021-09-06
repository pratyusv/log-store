#include "LogIterator.h"
#include "LogRecord.h"
#include <cstdint>


LogIterator::LogIterator(LogWriter* _pLogWriter) :
    pLogWriter(_pLogWriter), currentIndex(0) {}


LogIterator::LogIterator(const LogIterator& other) {
    pLogWriter = other.pLogWriter;
    currentIndex = other.currentIndex;
}

bool LogIterator::operator==(const LogIterator& it) {
    return this->getCurrentRecordIndex() == it.getCurrentRecordIndex();
}


bool LogIterator::operator!=(const LogIterator& it) {
    return this->getCurrentRecordIndex() != it.getCurrentRecordIndex();
}

LogRecord LogIterator::operator*() {
    return this->getCurrentRecord();
}

void LogIterator::operator++(int) {
    currentIndex++;
}

void LogIterator::operator++() {
    currentIndex++;
}

LogIterator LogIterator::operator+(uint64_t offset) const {
    LogIterator it(*this);
    it.LogIteratorBegin();
    it.currentIndex += offset;
    return it;
}

uint64_t LogIterator::getCurrentRecordIndex() const {
    return currentIndex;
}

LogRecord LogIterator::getCurrentRecord() {
    LogRecord record;
    if (!pLogWriter->ReadRecord(currentIndex, record)) {
        currentIndex = pLogWriter->GetEndRecordIndex() - 1;
    }
    return record;
}

void LogIterator::LogIteratorBegin() {
    currentIndex = pLogWriter->GetStartRecordIndex();
}

void LogIterator::LogIteratorEnd() {
    currentIndex = pLogWriter->GetEndRecordIndex();
}

