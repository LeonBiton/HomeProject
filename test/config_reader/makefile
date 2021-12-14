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

main: main.cpp $(SRC)/config_reader.cpp 

clean:
	$(RM) $(TARGET)


.PHONY: clean check