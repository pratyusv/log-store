#include <fstream>
#include <iostream>
#include <unistd.h>
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


std::vector<char> LogWriter::ProcessRecord(LogRecord& record)
{
    std::string currentTime = GetCurrentTimeStamp();
    
    std::vector<char> data = std::vector<char>(currentTime.begin(), currentTime.end());
    std::vector<char> recordMsg = record.getMessage();
    data.insert(data.end(),recordMsg.begin(), recordMsg.end());
    return data;
}

/*
    WriteTempRecordInStore: Write File in Temporary Storage. 
    Input: record: Log Record, fileName: File Name
    Output: bool
*/
bool LogWriter::WriteTempRecordInStore(LogRecord& record, char* fileName) {
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

bool LogWriter::AppendRecord(LogRecord& record) {
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
    Input: index: Index of the record
    Output: bool
*/
bool LogWriter::TruncateRecord(const uint64_t& index) {

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