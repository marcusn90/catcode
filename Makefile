# build_clang:
# 	clang -std=c11 -Wall -pedantic *.c -o dist/main_clang -lSDL2 -lSDL2_ttf

build:
	gcc *.c -o dist/main_gcc -lSDL2 -lSDL2_ttf

# dev_clang: build_clang
# 	./dist/main_clang

dev: build
	./dist/main_gcc

debug:
	gcc -g *.c -o dist/main_debug -lSDL2 -lSDL2_ttf