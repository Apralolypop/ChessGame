CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g

SFML_CFLAGS := $(shell pkg-config --cflags sfml-graphics sfml-window sfml-system 2>/dev/null)
SFML_LIBS := $(shell pkg-config --libs sfml-graphics sfml-window sfml-system 2>/dev/null)

ifeq ($(strip $(SFML_LIBS)),)
	SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-system
endif


.PHONY: all clean

all: chess engine chessgame


# ====================
# Normal GUI chess
# ====================

chess: main_gui.o board.o gui.o
	$(CXX) main_gui.o board.o gui.o -o chess $(SFML_LIBS)


# ====================
# GUI + AI engine
# ====================

engine: main_gui_engine.o board.o gui.o engine.o
	$(CXX) main_gui_engine.o board.o gui.o engine.o -o engine $(SFML_LIBS)


# ====================
# NO GUI TEST
# ====================
chessgame: main.o board.o
	$(CXX) main.o board.o -o chessgame

# Compile same GUI twice
# one normal
# one with ENGINE_ENABLED

main_gui.o: main_gui.cpp board.h gui.h
	$(CXX) $(CXXFLAGS) -c main_gui.cpp


main_gui_engine.o: main_gui.cpp board.h gui.h engine/engine.h
	$(CXX) $(CXXFLAGS) -DENGINE_ENABLED -c main_gui.cpp -o main_gui_engine.o

main.o: main.cpp board.h
	$(CXX) $(CXXFLAGS) -c main.cpp
# ====================
# Shared files
# ====================

board.o: board.cpp board.h pieces.h
	$(CXX) $(CXXFLAGS) -c board.cpp


gui.o: gui.cpp gui.h board.h pieces.h
	$(CXX) $(CXXFLAGS) $(SFML_CFLAGS) -c gui.cpp


engine.o: engine/engine.cpp engine/engine.h board.h pieces.h
	$(CXX) $(CXXFLAGS) -Iengine -c engine/engine.cpp -o engine.o



clean:
	rm -f *.o chess engine chessgame
	del /Q *.o chess.exe engine.exe chessgame.exe