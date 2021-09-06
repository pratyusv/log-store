#ifndef LOGWRITER_H
#define LOGWRITER_H

#include "LogRecord.h"
#include <cstdint>
#include <mutex>

class LogWriter {
    protected:
        std::string sLogStoreName;
        uint64_t startRecordIndex;
        uint64_t endRecordIndex;

        std::mutex _mutex;
    
    private:
        bool WriteTempRecordInStore(LogRecord& record,char* fileName);
    
    public:
        LogWriter(std::string LogStoreName);
        ~LogWriter();

        uint64_t GetStartRecordIndex() const;
        uint64_t GetEndRecordIndex() const;
        uint64_t GetNumOfRecords() const;


        std::vector<char> ProcessRecord(LogRecord& record);
        bool AppendRecord(LogRecord& record);
        bool ReadRecord(const uint64_t& recordIndex, LogRecord& record);
        bool TruncateRecord(const uint64_t& position);
};

#endif