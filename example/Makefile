CC = clang++

CFLAGS = -lncurses -lsimple_caro -latomic

SRCS = ./example.cpp

APP = ./app

all:
	$(CC) -o $(APP) $(SRCS) $(CFLAGS)

clean:
	rm -f $(APP)