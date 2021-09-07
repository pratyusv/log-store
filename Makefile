
BUILD := executable
INCDIR = ./include
SRCDIR = src
OBJS_TEST = utils.o
OBJS = utils.o  main.o
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

main.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) main.cpp

clean:
	clear
	rm -f *.config
	rm -f *.log
	rm -f *.o main
