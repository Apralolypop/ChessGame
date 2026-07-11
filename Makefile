CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g

# SFML flags via pkg-config (works if libsfml-dev / sfml is installed and
# pkg-config knows about it). Falls back to the plain -l flags below if
# pkg-config can't find it.
SFML_CFLAGS := $(shell pkg-config --cflags sfml-graphics sfml-window sfml-system 2>/dev/null)
SFML_LIBS   := $(shell pkg-config --libs sfml-graphics sfml-window sfml-system 2>/dev/null)
ifeq ($(strip $(SFML_LIBS)),)
	SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-system
endif

.PHONY: all clean

all: chessgame chess

# ---- Console version (your original) ----
chessgame: main.o board.o
	$(CXX) main.o board.o -o chessgame

main.o: main.cpp board.h pieces.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# ---- GUI version ----
chess: main_gui.o board.o gui.o
	$(CXX) main_gui.o board.o gui.o -o chess $(SFML_LIBS)

main_gui.o: main_gui.cpp board.h gui.h
	$(CXX) $(CXXFLAGS) -c main_gui.cpp

gui.o: gui.cpp gui.h board.h
	$(CXX) $(CXXFLAGS) $(SFML_CFLAGS) -c gui.cpp

# ---- Shared ----
board.o: board.cpp board.h pieces.h
	$(CXX) $(CXXFLAGS) -c board.cpp

clean:
	rm -f *.o chessgame chess