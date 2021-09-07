#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <cstdint>
#include <mutex>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include "utils.h"

template<typename T>
class LogWriter {

protected:
    std::string sLogStoreName;
    uint64_t startRecordIndex;
    uint64_t endRecordIndex;

    std::mutex _mutex;
    
private:
    bool WriteTempRecordInStore(T& record,char* fileName);
    
public:
    LogWriter(std::string storeName) : 
        sLogStoreName(storeName), startRecordIndex(0), endRecordIndex(0) {}

    ~LogWriter() {}

    uint64_t GetStartRecordIndex() const { return startRecordIndex; }
    uint64_t GetEndRecordIndex() const { return endRecordIndex; }
    uint64_t GetNumOfRecords() const { return endRecordIndex - startRecordIndex; }



    std::vector<char> ProcessRecord(T& record);
    bool AppendRecord(T& record);
    bool ReadRecord(const uint64_t& recordIndex, std::string& record);
    bool TruncateRecord(const uint64_t& position);
};


template<typename T>
std::vector<char> LogWriter<T>::ProcessRecord(T& record)
{

    std::string strMsg = record.FormatToString();
    std::vector<char> data = std::vector<char>(strMsg.begin(), strMsg.end());
    return data;
}


/*
    WriteTempRecordInStore: Write File in Temporary Storage. 
    Input: record: Log Record, fileName: File Name
    Output: bool
*/
template<typename T>
bool LogWriter<T>::WriteTempRecordInStore(T& record, char* fileName) {
    std::ofstream file;

    
    std::vector<char> data = ProcessRecord(record);
    const uint64_t bytesToWrite = data.size();

    if (0 == bytesToWrite || bytesToWrite > MAX_RECORD_SIZE ||  bytesToWrite > GetFreeSpace()) {
        return false;
    }


    int fd = mkstemp(fileName);   

    if (-1 == fd) {
        return false;
    }

    int fs = write(fd, data.data(), bytesToWrite);
    if (-1 == fs) {
        return false;
    }

    return true;
}

/*
    AppendRecord: Write Log to Log Store 
    Input: record: Log Record
    Output: bool
*/
template<typename T>
bool LogWriter<T>::AppendRecord(T& record) {
    char fileName[] = "/tmp/LogStore_XXXXXX";

    if (!WriteTempRecordInStore(record, fileName)) {
        return false;
    }

    // Acquire lock to create file and increase end Index.
    std::lock_guard<std::mutex> lock(_mutex);
    
    if (!RenameFileName(std::string(fileName), GetFileName(sLogStoreName, LOG_EXTENSION, endRecordIndex))) {
        return false;
    }

    endRecordIndex++;
    return true;
}

/*
    ReadRecord: Read Log Record from Log Store 
    Input: recordIndex: Index of the record, record: ptr to the record to be read.
    Output: bool
*/
template<typename T>
bool LogWriter<T>::ReadRecord(const uint64_t& recordIndex, std::string& record) {
    std::ifstream file;
    uint64_t size = 0;
    {
        // Aquire lock to read file.
        std::lock_guard<std::mutex> lock(_mutex);

        if (recordIndex < startRecordIndex || recordIndex >= endRecordIndex) {
            return false;
        }

        size = GetFileSize(sLogStoreName, recordIndex);
        
        std::string _fileName = GetFileName(sLogStoreName, LOG_EXTENSION, recordIndex); 
        file.open(_fileName.c_str(), std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            return false;
        }
    }

    // Read file.
    std::vector<char> message(size, 0);
    file.read(message.data(), size);

    record = std::string(message.begin(), message.end());
    return true;
}


/*
    TruncateRecord: Delete Log from Log Store
    Input: index: Index of the record
    Output: bool
*/
template<typename T>
bool LogWriter<T>::TruncateRecord(const uint64_t& index) {

    uint64_t tempIndex = 0;
    {
        // Aquire Lock to trucate the file
        std::lock_guard<std::mutex> lock(_mutex);
        tempIndex = startRecordIndex;

        if (index < startRecordIndex || index >= endRecordIndex) {
            return false;
        }
        startRecordIndex = index + 1;
    }

    uint64_t recordIndex = tempIndex;

    // Truncate the files from current index to last index.
    while (recordIndex <= index) {
        if (!DeleteFile(sLogStoreName, recordIndex)) {
            return false;
        }
        recordIndex++;
    }

    return true;
}
#endif