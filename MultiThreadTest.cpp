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


class ThreadTest {
  private:
    LogStore<StringBlob> *log_;
    std::mutex lock_;
    std::vector<std::vector<std::string>> out_;

  public:
    ThreadTest(LogStore<StringBlob> *log): log_(log) {}

    void writeThread() {
        for (uint64_t i = 1; i <= 100; i++) {
            log_->Append(StringBlob(randomString(100) + IntToString(i)));
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    void readThread() {
        std::vector<std::string> records;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        for (LogIterator<StringBlob> itr = log_->Begin(); itr != log_->End(); itr++) {
            records.push_back(*itr);
        }

        std::lock_guard<std::mutex> lock(lock_);
        out_.push_back(records);
    }

    std::vector<std::vector<std::string>> get_out() const {
        return std::move(out_);
    }
};

bool checkReadData(std::vector<std::vector<std::string>> &&out) {
    for (auto &vec : out) {
        uint64_t prev_count = 0;

        for (auto &e : vec) {
            size_t pos = e.find_first_of("0123456789");
            if (pos == std::string::npos) {
                return false;
            }

            std::string counter = e.substr(pos, e.size() - pos);
            uint64_t count = StringToInt(counter);

            if (count <= prev_count) {
                return false;
            }

            prev_count = count;
        }
    }

    return true;
}

void multiThreadinTest() {
    LogStore<StringBlob> log("MultiThread_TestLog");
    ThreadTest t(&log);

    std::vector<std::thread> workers;

    // create write thread
    workers.push_back(std::thread(&ThreadTest::writeThread, &t));

    // create multiple read threads
    for (uint64_t i = 0; i < 10; i++) {
        workers.push_back(std::thread(&ThreadTest::readThread, &t));
    }

    // wait for all threads to finish
    for (auto& thread : workers) {
        thread.join();
    }

    // check read data
    assert(checkReadData(t.get_out()) == true);

    // empty log store
    uint64_t pos = log.GetIndex();
    log.Truncate(pos-1);

    assert(log.GetSize() == 0);
}



int main() {
    multiThreadinTest();
    return 0;
}