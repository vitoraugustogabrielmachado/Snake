CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lncurses       # linka ncurses
TARGET = jogo
SRC = snake.c jogo.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)