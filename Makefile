all: compile link

compile:
	g++ -I src/include -c Gomoku.cpp
link:
	g++ Gomoku.o -o Gomoku -L src/lib -l sfml-audio -l sfml-graphics -l sfml-network -l sfml-system -l sfml-window