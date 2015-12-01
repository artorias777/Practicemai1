all: func.cpp 
	g++ -ggdb `pkg-config --cflags opencv` -o hhcircle func.cpp `pkg-config --libs opencv`
