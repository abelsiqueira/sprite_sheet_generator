EXEC = sprite_sheet_generator

all:
	g++ -o $(EXEC) main.cpp -Wall -Wextra -lallegro -lallegro_image -ggdb

install:
	cp $(EXEC) /usr/bin
