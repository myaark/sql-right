CC = gcc
CFLAGS = -g -O0 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude
LDLIBS = -lreadline
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)
OUT_DIR = bin
OUT = $(OUT_DIR)/sql-right

.PHONY: all build run clean install-deps

all: build

build: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDLIBS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

run: build
	./$(OUT)

install-deps:
	@if command -v apt-get >/dev/null 2>&1; then \
		apt-get update && apt-get install -y libreadline-dev; \
	elif command -v dnf >/dev/null 2>&1; then \
		dnf install -y readline-devel; \
	elif command -v pacman >/dev/null 2>&1; then \
		pacman -S --needed --noconfirm readline; \
	else \
		echo "No supported package manager found. Install the Readline development package manually." >&2; \
		exit 1; \
	fi

clean:
	rm -f $(OUT)