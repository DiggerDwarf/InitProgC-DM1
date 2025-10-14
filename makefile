
all: build run

build:
	gcc multi_dames.c -o multi_dames -Wall -Wextra -Werror -std=c17

clean:
	rm multi_dames -f

run:
	./multi_dames

release:
	zip KUNTZ_LI.zip multi_dames.c makefile
