# make file for simple server

CXX ?= g++

CXXFLAGS ?= -Wall \
	-std=c++11

CXXFLAGS_DEBUG := -g3 -O

CXXFLAGS_RELEASE := -O2 -DNDEBUG

#the default target is debug
all: debug

#debug section
debug: CXXFLAGS += $(CXXFLAGS_DEBUG)
debug: app

#release section
release: CXXFLAGS += $(CXXFLAGS_DEBUG)
release: app

#remove all files
clean:
	rm test.o app

app: test.o
	$(CXX) $^ -o $@ $(CXXFLAGS) -I

test.o:
	$(CXX) -c test.cpp -o test.o $(CXXFLAGS) -I 

.PHONY: debug release clean
