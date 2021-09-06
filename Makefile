
BUILD := executable
INCDIR = ./include
SRCDIR = src
OBJS_TEST = utils.o  LogRecord.o LogIterator.o LogWriter.o  LogStore.o
OBJS = utils.o LogRecord.o LogWriter.o LogIterator.o LogStore.o main.o
CXX = g++
DEBUG = -g
LFLAGS = -Wall $(DEBUG) -lpthread
cxxflags.test := -Wall -O1 -c $(DEBUG) -pthread -std=c++14 #-fsanitize=thread
cxxflags.executable := -Wall -O3 -c -std=c++14
CXXFLAGS := ${cxxflags.${BUILD}}


all: $(BUILD)

executable: $(OBJS)
	@echo "Build executable..."
	$(CXX) $(OBJS) -o main $(LFLAGS)

utils.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(SRCDIR)/utils.cpp

LogRecord.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(SRCDIR)/LogRecord.cpp

LogWriter.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(SRCDIR)/LogWriter.cpp

LogIterator.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(SRCDIR)/LogIterator.cpp

LogStore.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(SRCDIR)/LogStore.cpp

main.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) main.cpp

clean:
	rm -f *.o main
