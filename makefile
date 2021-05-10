all:
	g++ Gomoku.cpp --std=c++17 $(shell pkg-config --cflags --libs sdl2) -lSDL2_image -lSDL2_ttf -o Gomoku.exe

install:
	sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev