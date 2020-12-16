OUT=./dist/main
LIBS=-lSDL2 -lSDL2_ttf
build:
	gcc *.c -o $(OUT) $(LIBS)

run: build
	$(OUT)

debug:
	gcc -g *.c -o dist/main_debug $(LIBS)