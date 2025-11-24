.PHONY: run clean

honeyfw.o: honeyfw.c
	gcc -o honeyfw.o -c $^

build: honeyfw.o game.c
	gcc -o build $^ -lSDL2_image $(shell sdl2-config --cflags) $(shell sdl2-config --libs)

run: build
	./build

clean:
	rm honeyfw.o
	rm build