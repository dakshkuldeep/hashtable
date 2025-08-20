CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC
NAME = oaht
SRC = $(NAME).c
HDR = $(NAME).h
OBJ = build/$(NAME).o
SHARED = build/liboaht.so

.PONY: all clean install builddir

all: $(SHARED)

# make build folder to hold builds
builddir:
	mkdir -p build

#create object file
$(OBJ): $(SRC) $(HDR) | builddir
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

#create shared library
$(SHARED): $(OBJ)
	$(CC) -shared -o $@ $^

clean: 
	rm -rf build/*

#install system wide (sudo)
install: all
	cp $(SHARED) /usr/local/lib/
	cp $(HDR) /usr/local/include/
	ldconfig
