
all: build run

build:
	gcc multi_dames.c -o multi_dames -Wall -Wextra -Werror -std=c17

clean:
	rm multi_dames

run:
	./multi_dames
