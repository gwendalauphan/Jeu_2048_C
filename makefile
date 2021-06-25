cc = gcc -Wall
FILES = obj/main.o obj/function.o obj/graphics.o obj/save.o#liste des fichiers sources avec .o
ifeq ($(OS), Windows_NT)
	OPT = -lm -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
	COM = del obj\*.o
else
	OPT = `pkg-config --libs --cflags sdl2 SDL2_ttf`
	COM = rm obj/*.o
endif

all: 2048 #commande pour compiler et lancer le jeu
	make clean
	./2048

2048: $(FILES) #commande pour compiler le jeu
	$(cc) $^ -o $@ $(OPT)

obj/%o: src/%c #commande de création des .o avec les .c
	$(cc) -c $^ -o $@ $(OPT)


clean: #commande pour retirer tous les fichiers objects
	$(COM)
