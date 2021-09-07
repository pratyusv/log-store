
#include <iostream>
#include <sstream>
#include "LogWriter.h"
#include "LogStore.h"
#include "LogIterator.h"
#include "./CustomBlob/StringBlob.h"
#include "./CustomBlob/ErrorBlob.h"

int main() {
    // LogStore<StringBlob> log("system_log");
    // log.Append(StringBlob("hello_how_are_you"));
    // log.Append(StringBlob("_great"));


    LogStore<ErrorBlob> log("system_log");
    log.Append(ErrorBlob(11, "Null pointer Exception", 34, "cpp"));
    log.Append(ErrorBlob(11, "Divide by Zero", 34, "java"));
    log.Append(ErrorBlob(11, "Null pointer Exception", 34, "cpp"));
    log.Append(ErrorBlob(11, "Divide by Zero", 34, "java"));
    log.Append(ErrorBlob(11, "Null pointer Exception", 34, "cpp"));
    log.Append(ErrorBlob(11, "Divide by Zero", 34, "java"));
    log.Append(ErrorBlob(11, "Null pointer Exception", 34, "cpp"));
    log.Append(ErrorBlob(11, "Divide by Zero", 34, "java"));

    

    for (LogIterator<ErrorBlob> itr = log.Begin(); itr != log.End(); itr++) {
        std::cout << (*itr) << std::endl;
    }

    // truncate log store
    uint64_t pos = log.GetIndex();
    log.Truncate(3);

    return 0;
}