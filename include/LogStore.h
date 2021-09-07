#ifndef LOGSTORE_H
#define LOGSTORE_H

#include <fstream>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include "LogWriter.h"
#include "LogIterator.h"
#include "utils.h"
#include "Constants.h"


template<typename T>
class LogStore : public LogWriter<T> {
private:
    std::string logStoreName;

    bool SaveLogInfo();
    bool LoadLogInfo();

public:

    LogStore(const std::string& _storeName): 
        LogWriter<T>(_storeName), logStoreName(_storeName) {
            LoadLogInfo();
    } 

    ~LogStore() {
        SaveLogInfo();
    }

    LogIterator<T> Begin() {
        LogIterator<T> it(this);
        it.LogIteratorBegin();
        return it;
    }

    LogIterator<T> End() {
        LogIterator<T> it(this);
        it.LogIteratorEnd();
        return it;
    }

    bool Append(T&& record) {
        return this->AppendRecord(record);
    } 
    
    bool Truncate(const uint64_t& index) {
        return this->TruncateRecord(index);
    }

    uint64_t GetIndex() const {
        return this->GetEndRecordIndex();
    }

    uint64_t GetSize() const {
        return this->GetNumOfRecords();
    }
};

template<typename T>
bool LogStore<T>::LoadLogInfo() {
    std::ifstream file;
    std::string fileName = GetFileName(logStoreName, CONFIG_EXTENSION).c_str();
    file.open(fileName, std::ios::in);
    if (!file.is_open()) {
        return false;
    }

    std::vector<std::string> data;
    std::string line;

    while (getline(file, line)) {
        data.push_back(line);
    }

    if (data.size() != 2) {
        return false;
    }

    this->startRecordIndex = StringToInt(data[0]);
    this->endRecordIndex = StringToInt(data[1]);

    return 0;
}

template<typename T>
bool LogStore<T>::SaveLogInfo() {
    if(this->startRecordIndex == this->endRecordIndex) {
        return true;
    }

    std::ofstream file;
    std::string fileName = GetFileName(logStoreName, CONFIG_EXTENSION).c_str();
    file.open(fileName, std::ios::out);
    if (!file.is_open()) {
        return false;
    }

    std::vector<std::string> data = {IntToString(this->startRecordIndex), IntToString(this->endRecordIndex)};

    for (uint64_t i = 0; i < data.size(); i++) {
        std::string line = data[i] + std::string("\n");
        file.write(line.c_str(), line.size());
        if (!file.good()) {
            return false;
        }
    }

    return true;

}

#endif