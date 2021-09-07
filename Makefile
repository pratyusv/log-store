
BUILD := executable
INCDIR = ./include
SRCDIR = src
OBJS = utils.o  main.o 
OBJS_TEST = utils.o  NonThreadedTest.o
OBJS_MTTEST = utils.o MultiThreadTest.o
CXX = clang++
DEBUG = -g
LFLAGS = -Wall $(DEBUG) -lpthread
cxxflags.test := -Wall -O1 -c $(DEBUG) -pthread -std=c++14 #-fsanitize=thread
cxxflags.executable := -Wall -O3 -c -std=c++14
CXXFLAGS := ${cxxflags.${BUILD}}


all: $(BUILD)

test: $(OBJS_TEST)
	@echo "Build test..."
	$(CXX) $(OBJS_TEST) -o test $(LFLAGS)

mt_test: $(OBJS_MTTEST)
	@echo "Build MultiThread test..."
	$(CXX) $(OBJS_MTTEST) -o mt_test $(LFLAGS)

executable: $(OBJS)
	@echo "Build executable..."
	$(CXX) $(OBJS) -o main $(LFLAGS)

utils.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(SRCDIR)/utils.cpp

main.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) main.cpp

NonThreadedTest.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) NonThreadedTest.cpp

MultiThreadTest.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) MultiThreadTest.cpp

clean:
	clear
	rm -f *.o *.log *.config main mt_test test
