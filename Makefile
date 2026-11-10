CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2 -Iinclude
LDFLAGS ?= -lSDL3

SRC = src/main.c src/chip8.c
TARGET_DIR = exe

ifeq ($(OS),Windows_NT)
    TARGET = $(TARGET_DIR)/chip8.exe
else
    TARGET = $(TARGET_DIR)/chip8
endif

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)
	@echo "Build concluída com sucesso: $(TARGET)"

clean:
	rm -f $(TARGET_DIR)/chip8 $(TARGET_DIR)/chip8.exe

.PHONY: all clean
