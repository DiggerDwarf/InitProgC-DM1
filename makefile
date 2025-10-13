
all: build run

build:
	gcc multi_dames.c -o multi_dames -Wall -Wextra -Werror -std=c17

run:
	./multi_dames
