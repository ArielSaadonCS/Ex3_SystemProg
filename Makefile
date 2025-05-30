
CC = clang++
CFLAGS = -Wall -std=c++17 -g

SRCS = Baron.cpp Game.cpp General.cpp Governor.cpp Judge.cpp Merchant.cpp Player.cpp Spy.cpp
GUI_SRCS = GameGUI.cpp
HEADERS = Game.hpp Player.hpp General.hpp Governor.hpp Judge.hpp Merchant.hpp Spy.hpp Baron.hpp
TEST_SRCS = $(filter-out main.cpp, $(SRCS))
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

.PHONY: all main test gui valgrind clean

all : main test gui

main: main.cpp $(SRCS) $(GUI_SRCS) $(HEADERS)
	$(CC) $(CFLAGS) main.cpp $(SRCS) -o main $(LIBS)
	

test: test.cpp $(TEST_SRCS) $(HEADERS)
	$(CC) $(CFLAGS) test.cpp $(TEST_SRCS) -o test
	

gui: GameGUI.cpp $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) GameGUI.cpp $(SRCS) -o gui $(LIBS)
valgrind: main test
	valgrind --leak-check=full ./main
	valgrind --leak-check=full ./test

clean: 
	rm -f main test *.o