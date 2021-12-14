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

main: main.cpp $(SRC)/event_router.cpp $(SRC)/event_task.cpp $(SRC)/guard.cpp $(SRC)/semaphore.cpp $(SRC)/threadpool.cpp $(SRC)/thread.cpp $(INL)/blocking_bounded_queue.hxx $(SRC)/thread_group.cpp $(SRC)/tcpclient.cpp $(SRC)/event_decoder.cpp $(SRC)/genreal_event.cpp $(SRC)/socket.cpp $(SRC)/tcpserver.cpp $(SRC)/ievent.cpp $(SRC)/topic.cpp $(SRC)/subscribers_manager.cpp $(SRC)/iagent.cpp $(SRC)/notifyer.cpp $(SRC)/utils.cpp -lpthread -ldl

clean:
	$(RM) $(TARGET)


.PHONY: clean check