build_clang:
	clang -std=c11 -Wall -pedantic *.c -o dist/main_clang -lSDL2 -lSDL2_ttf

build_gcc:
	gcc *.c -o dist/main_gcc -lSDL2 -lSDL2_ttf

dev_clang: build_clang
	./dist/main_clang

dev_gcc: build_gcc
	./dist/main_gcc

dev: dev_gcc

debug:
	gcc -g *.c -o dist/main_debug -lSDL2 -lSDL2_ttf