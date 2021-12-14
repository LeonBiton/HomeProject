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

main: main.cpp $(INC)/thread.hpp $(SRC)/guard.cpp $(SRC)/thread.cpp $(INC)/inl/atomic.hxx $(INC)/atomic.hpp $(SRC)/semaphore.cpp $(INC)/thread_group.hpp $(SRC)/thread_group.cpp $(INC)/funcadapter.hpp $(INC)/inl/funcadapter.hxx ../src/counter.cpp ../inc/counter.hpp -lpthread

clean:
	$(RM) $(TARGET)


.PHONY: clean check