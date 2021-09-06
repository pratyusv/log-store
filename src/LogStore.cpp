#include <fstream>
#include "LogStore.h"
#include "utils.h"
#include "Constants.h"

LogStore::LogStore(const std::string& _storeName): 
    LogWriter(_storeName), logStoreName(_storeName) {
        LoadLogInfo();
} 

LogStore::~LogStore() {
    SaveLogInfo();
}


LogIterator LogStore::Begin() {
    LogIterator it(this);
    it.LogIteratorBegin();
    return it;
}

LogIterator LogStore::End() {
    LogIterator it(this);
    it.LogIteratorEnd();
    return it;
}

bool LogStore::Append(LogRecord&& record) {
    return AppendRecord(record);
} 

bool LogStore::Truncate(const uint64_t& index) {
    return TruncateRecord(index);
}
uint64_t LogStore::GetIndex() const {
    return GetEndRecordIndex();
}


uint64_t LogStore::GetSize() const {
    return GetNumOfRecords();
}

bool LogStore::LoadLogInfo() {
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

    startRecordIndex = StringToInt(data[0]);
    endRecordIndex = StringToInt(data[1]);

    return 0;
}


bool LogStore::SaveLogInfo() {
    if(startRecordIndex == endRecordIndex) {
        return true;
    }

    std::ofstream file;
    std::string fileName = GetFileName(logStoreName, CONFIG_EXTENSION).c_str();
    file.open(fileName, std::ios::out);
    if (!file.is_open()) {
        return false;
    }

    std::vector<std::string> data = {IntToString(startRecordIndex), IntToString(endRecordIndex)};

    for (uint64_t i = 0; i < data.size(); i++) {
        std::string line = data[i] + std::string("\n");
        file.write(line.c_str(), line.size());
        if (!file.good()) {
            return false;
        }
    }

    return true;

}