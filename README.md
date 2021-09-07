# Log-Store
Logger to append, truncate, replay logs

## Log Store Homework Assignment

A log store is a record storage model where updates only need to append rather than seeking and writing. To load the records, the log store is read from a position and replayed back. The ordering of records ensure a consistent model. A log store would grow without bound unless it is maintained. One way reduce its size is to re-append the records to be preserved, then truncate the store. Truncating the store should release storage back to the operating system. However, not all data is required to be released, so long as the disk footprint remains roughly proportional to the data retained.

In C++ or C++14 (or higher), implement a simple log store with the following requirements and functions:

### Requirements:

    • The implementation must be thread safe
    • Records are variably sized opaque blobs of bytes anywhere from 1 byte to 32MB
    • The log store must store on disk (not an in-memory log store)
    • Truncation must be efficient and not require too much copying of data

### Functions:

> append(blob) -> position Append a record blob to the log store.

> get_position() -> position
                Gets the current position of the log store.

> truncate(position) ->
                Removes all data before the given position in the log store. Disk space used must remain roughly proportional to retained data size.

> replay(position, callback)
                Replays all record blobs from the position onward, invoking the callback for each blob visited



Note: The above function signatures are only illustrative. You may vary your interface in whatever way you think makes sense. For example, instead of a callback for replay(…), you may decide to use an iterator pattern.

---

## Solution Overview

1. `LogStore<T>`:
    * Creates a log store on the disk. The log store can store log records in custom format.
    * To minimize the latency each log record is stored as a single file on the disk.
2. An index is maintained to keep track of the number of records stored on disk.
3. `append(blob)`: Append creates a new temporay file. The record is stored in the temporary file and then the file is moved to the Log store location. Every append operation increaments the index.
4. `truncate(position)`: Deletes the files from the `starting index` to the index passed as parameter.
5. `LogIterator` iterates over the records.

To support Multi Threading, mutex lock is used in case of contention.

1. `read`:
    > lock --> open file --> unlock --> read file

2. `append`: 
    > create new temporary record -> lock --> increament current index --> rename the file --> unlock

3. `truncate`: the start index is a source of contention as other threads may manipulate it while the truncate operation is in process:

    > lock --> store the start index --> move the start index to the new position --> unlock --> delete the file.


---


## Class Overview

1. `LogStore` : Logstore provides APIs to create, append and iterate over log store
2. `LogIterator` : LogIterator provides APIs to iterate over logs
3. `LogWriter` : LogWriter stores, reads logs from the storage

---
## Requirements
1. [make](https://www.gnu.org/software/make/)
2. Clang++

---

## Run
```sh
$ make clean
$ make
$ ./main
```

## Make
1. `make clean`: cleans the current folder
2. `make test` : builds the test for single threading
3. `make mt_test`: builds the test for multi threading

---

## Running Tests

Basic Test:
```sh
$ make clean
$ make test
$ ./test
```

Multithread Test:

```sh
$ make clean
$ make mt_test
$ ./mt_test
```

----

## Blob Format

Custom format should inherit the `Formatter` class and implement the virtual `FormatToString`.

Example:

```cpp
class StringBlob: public Formatter {
    private:
        std::string message;
    public:
        StringBlob() : message("") {}
        StringBlob(std::string _msg) : message(_msg) {}

        std::string FormatToString() {
            return message;
        }
};
```

---

## Example Usage

```cpp   
    // Create LogStore
    LogStore<StringBlob> log("LogStore");

    // append Logs
    log.Append(StringBlob("Message1"));
    log.Append(StringBlob("Message2"));

    // iterate over logs
    for (LogIterator<StringBlob> itr = log.Begin(); itr != log.End(); itr++) {
        std::cout << (*itr) << std::endl;
    }

    // truncate log store
    uint64_t pos = log.GetIndex();
    log.Truncate(pos-1);
```
