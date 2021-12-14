TARGET=main

CXX=g++
CC=$(CXX)

CFLAGS= -g3 -pedantic

CXXFLAGS = -std=c++17
CXXFLAGS += -pedantic -Werror -Wall
CXXFLAGS += -g3

CPPFLAGS = -I../inc
CPPFLAGS += -I../../inc

SRC = ../../src
INC = ../../inc
INL = ../../inc/inl

check: $(TARGET)
	./$(TARGET)

main: main.cpp $(SRC)/threadpool.cpp $(SRC)/barrier.cpp $(SRC)/guard.cpp $(SRC)/thread.cpp $(SRC)/semaphore.cpp $(INC)/inl/atomic.hxx $(INC)/atomic.hpp $(SRC)/thread_group.cpp $(INC)/inl/funcadapter.hxx -lpthread

clean:
	$(RM) $(TARGET)


.PHONY: clean check