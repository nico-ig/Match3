CC = gcc
CFLAGS = -g -std=c99 -Wall -Wextra

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

ALLEGRO = `pkg-config allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 --libs --cflags`

TARGET = match3

all: $(TARGET)

r: all
	./$(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(ALLEGRO)

clean:
	@rm -f src/*.o vgcore* 

purge: clean
	@rm -f $(TARGET)
