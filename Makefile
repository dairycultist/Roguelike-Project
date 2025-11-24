.PHONY: run clean

build: *.c
	gcc -o build $^ -lSDL2_image $(shell sdl2-config --cflags) $(shell sdl2-config --libs)

run: build
	./build

clean:
	rm build