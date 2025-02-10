CC = gcc

CFLAGS = -Wall -pedantic
SRC = collector.c command.c debug_printing.c game_actions.c game_loop.c game.c graphic_engine.c space.c object.c player.c game_reader.c entity.c link.c
OBJ = collector.o command.o debug_printing.o game_actions.o game_loop.o game.o graphic_engine.o space.o object.o player.o libscreen.a entity.o game_reader.o link.o
EXE = anthill

#link: $(OBJ)
#	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

#debug: $(SRC) ARREGLAR
#	$(CC) $(CFLAGS) -g $(OBJ) -o anthilldebug

all: (EXE)

(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) libscreen.a

game_loop.o:	game_loop.c command.h game_actions.h graphic_engine.h game.c game_reader.c
	$(CC) $(CFLAGS) -c game_loop.c

game.o:	game.c game.h
	$(CC) $(CFLAGS) -c game.c

command.o:	command.c command.h
	$(CC) $(CFLAGS) -c command.c

space.o:	space.c space.h
	$(CC) $(CFLAGS) -c space.c

game_actions.o:	game_actions.c game_actions.h
	$(CC) $(CFLAGS) -c game_actions.c

graphic_engine.o:	graphic_engine.c graphic_engine.h
	$(CC) $(CFLAGS) -c graphic_engine.c

object.o:	object.c object.h
	$(CC) $(CFLAGS) -c object.c

entity.o:	entity.c entity.h
	$(CC) $(CFLAGS) -c entity.c
	
player.o:	player.c player.h entity.h
	$(CC) $(CFLAGS) -c player.c

collector.o:	collector.c collector.h
	$(CC) $(CFLAGS) -c collector.c

debug_printing.o:	debug_printing.c debug_printing.h
	$(CC) $(CFLAGS) -c debug_printing.c

game_reader.o : game_reader.c game_reader.h space.h types.h game.c
	$(CC) $(CFLAGS) -c game_reader.c

link.o : link.c link.h types.h entity.c game.c
	$(CC) $(CFLAGS) -c link.c


.PHONY:	clean compile run
clean:
	rm -f *.o $(EXE)

compile:
	$(CC) $(CFLAGS) -c $(SRC)

link:
	$(CC) $(CFLAGS) -o $(OBJ)

run:	(EXE)
	./anthill anthill.dat
	
#PARA COMPILAR SOLO LO NECESARIO SE USA LA SIGUIENTE SINTAXIS sustituyendo: (REVISAR)
#.o: .c (dependencias)
#	$(CC) $(CFLAGS) -c $@ $^

# $@ devuelve lo que hay a la izquierda de los :, $^ devuelve todas las dependencias

# A la hora de añadir dependencias de librerias de otra carpeta se puede usar:
# por ejemplo y siestoy compilando en src y hay una carpeta contigua llamada include
# gcc -I ../include (PARA BUSCAR EN LA CARPETA ../include)