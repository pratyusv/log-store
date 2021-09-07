#include <iostream>
#include <string>
#include <cassert>
#include <thread>
#include <chrono>
#include <sstream>
#include <mutex>
#include <locale>
#include "LogStore.h"
#include "LogIterator.h"
#include "./include/utils.h"
#include "./CustomBlob/StringBlob.h"



void NonThreadedTest() {
    std::string out;

    std::cout <<"Creating Log Storage: TestLog" << std::endl;
    LogStore<StringBlob> log("TestLog");

    std::cout <<"==============================================" <<std::endl;
    std::cout <<"Appending Log Entries .."<< std::endl;
    log.Append(StringBlob("Unit"));
    log.Append(StringBlob("_Test"));
    log.Append(StringBlob("_is"));
    log.Append(StringBlob("_Successful"));
    std::cout <<"Log Entries Inserted"<< std::endl;
    std::cout <<"==============================================" <<std::endl;

    assert(log.GetSize() == 4);

    std::cout <<"Saved Logs: " << std::endl; 
    for (LogIterator<StringBlob> itr = log.Begin(); itr != log.End(); itr++) {
        out += (*itr); 
        std::cout << (*itr) << std::endl;
    }
    std::cout <<"==============================================" <<std::endl;
    std::cout <<"Appended Logs: " << out << std::endl; 

    assert(out == "Unit_Test_is_Successful");
    std::cout <<"==============================================" <<std::endl;

    std::cout <<"Truncate Logs at position 0" << std::endl;
    log.Truncate(0);
    assert(log.GetSize() == 3);
    
    std::cout <<"Saved Logs: " << std::endl;
    out.clear();
    for (LogIterator<StringBlob> itr = log.Begin(); itr != log.End(); itr++) {
        out += (*itr); 
    }
    std::cout << out << std::endl;

    assert(out == "_Test_is_Successful");


    std::cout <<"==============================================" <<std::endl;
    std::cout << "Append Log"<< std::endl;
    log.Append(StringBlob("_OK"));

    assert(log.GetSize() == 4);

    out.clear();
    for (LogIterator<StringBlob> itr = log.Begin(); itr != log.End(); itr++) {
        out += (*itr); 
    }
    std::cout <<"Saved Logs: " << std::endl;
    std::cout << out << std::endl;

    assert(out == "_Test_is_Successful_OK");

    std::cout <<"==============================================" <<std::endl;
    std::cout <<"Truncate all logs" << std::endl;
    uint64_t idx = log.GetIndex();
    log.Truncate(idx-1);

    assert(log.GetSize() == 0);
    std::cout <<"==============================================" <<std::endl;
}

int main() {

    NonThreadedTest();
    std::cout <<"Test Successful" << std::endl;
    return 0;
}