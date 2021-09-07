
#include <iostream>
#include <sstream>
#include "LogWriter.h"
#include "LogStore.h"
#include "LogIterator.h"
#include "./customBlob/StringBlob.h"
#include "./customBlob/ErrorBlob.h"

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
    log.Truncate(pos-1);

    return 0;
}