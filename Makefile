# Install
BIN = demo

# Flags
CFLAGS = -std=c89 -pedantic -O2 

SRC = main.c
OBJ = $(SRC:.c=.o)

ifeq ($(OS),Windows_NT)
BIN := $(BIN).exe
LIBS = -lmingw32 -lSDLmain -lSDL -lm
else
LIBS = -lSDL -lSDL_gfx -lm
endif

$(BIN): nuklear/nuklear.h
	@mkdir -p bin
	rm -f bin/$(BIN) $(OBJS)
	$(CC) $(SRC) $(CFLAGS) -o bin/$(BIN) $(LIBS)

nuklear/nuklear.h:
	git submodule update --init

test: $(BIN)
	bin/$(BIN)