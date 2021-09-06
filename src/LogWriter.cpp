#include <fstream>
#include <iostream>
#include "utils.h"
#include "Constants.h"
#include "LogWriter.h"
#include "Constants.h"



LogWriter::LogWriter(std::string storeName) : 
    sLogStoreName(storeName), startRecordIndex(0), endRecordIndex(0) {}


LogWriter::~LogWriter() {}

uint64_t LogWriter::GetStartRecordIndex() const { return startRecordIndex; }
uint64_t LogWriter::GetEndRecordIndex() const { return endRecordIndex; }
uint64_t LogWriter::GetNumOfRecords() const { return endRecordIndex - startRecordIndex; }

/*
    WriteTempRecordInStore: Write File in Temporary Storage. 
    Input: record: Log Record, fileName: File Name
    Output: bool
*/
bool LogWriter::WriteTempRecordInStore(LogRecord& record, const std::string& fileName) {
    std::ofstream file;
    std::vector<char> data = record.getMessage();
    const uint64_t bytesToWrite = data.size();

    if (0 == bytesToWrite || bytesToWrite > MAX_RECORD_SIZE ||  bytesToWrite > GetFreeSpace()) {
        return false;
    }

    file.open(fileName.c_str(), std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    file.write(data.data(), bytesToWrite);
    if (!file.good()) {
        return false;
    }

    return true;
}

/*
    AppendRecord: Write Log to Log Store 
    Input: record: Log Record
    Output: bool
*/

bool LogWriter::AppendRecord(LogRecord& record) {
    std::string fileName = GetTempFileName();

    if (!WriteTempRecordInStore(record, fileName)) {
        return false;
    }

    // Acquire lock to create file and increase end Index.
    std::lock_guard<std::mutex> lock(_mutex);

    if (!RenameFileName(fileName, GetFileName(sLogStoreName, LOG_EXTENSION, endRecordIndex))) {
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
bool LogWriter::ReadRecord(const uint64_t& recordIndex, LogRecord& record) {
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
    file.read(record.getMessageData(size), size);
    return true;
}


/*
    TruncateRecord: Delete Log from Log Store
    Input: position: Index of the record
    Output: bool
*/
bool LogWriter::TruncateRecord(const uint64_t& position) {

    uint64_t tempIndex = 0;
    {
        // Aquire Lock to trucate the file
        std::lock_guard<std::mutex> lock(_mutex);
        tempIndex = startRecordIndex;

        if (position < startRecordIndex || position >= endRecordIndex) {
            return false;
        }
        startRecordIndex = position + 1;
    }

    uint64_t recordIndex = tempIndex;

    // Truncate the files from current index to last index.
    while (recordIndex <= position) {
        if (!DeleteFile(sLogStoreName, recordIndex)) {
            return false;
        }
        recordIndex++;
    }

    return true;
}