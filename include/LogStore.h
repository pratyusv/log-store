#ifndef LOGSTORE_H
#define LOGSTORE_H

#include <cstdint>
#include <string>
#include "LogWriter.h"
#include "LogIterator.h"


class LogStore : public LogWriter {
    private:
        std::string logStoreName;

        bool SaveLogInfo();
        bool LoadLogInfo();

    public:
        LogStore(const std::string&);
        ~LogStore();

        LogIterator Begin();
        LogIterator End();

        bool Append(LogRecord&& record);
        bool Truncate(const uint64_t& index);

        uint64_t GetIndex() const;
        uint64_t GetSize() const;

};

#endif