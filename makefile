CC = g++
LIBS = -lraylib -lm -lpthread -ldl -lrt -lX11
O = -o main
SRC = src/main.cpp

compile:
	$(CC) $(SRC) $(O) $(LIBS)
	make run

run:
	./main