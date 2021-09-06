#include "LogStore.h"
#include "LogIterator.h"
#include "LogFormatter.h"
#include <iostream>

int main() {
    LogStore log("system_log");
    log.Append(StringRecord("hello_how_are_you"));
    log.Append(StringRecord("_great"));
    log.Append(StringRecord("_amazing"));
    log.Append(StringRecord("_yes"));

    for (LogIterator itr = log.Begin()+1; itr != log.End(); itr++) {
        std::cout << LogFormatter::toString(*itr) << std::endl;
    }

    // empty log store
    uint64_t pos = log.GetIndex();
    log.Truncate(pos-1);

    return 0;
}