CC = gcc

CFLAGS = -Wall -pedantic
SRC = collector.c command.c debug_printing.c game_actions.c game_loop.c game.c graphic_engine.c space.c object.c player.c game_reader.c entity.c link.c inventory.c collection.c
OBJ = collector.o command.o debug_printing.o game_actions.o game_loop.o game.o graphic_engine.o space.o object.o player.o libscreen.a entity.o game_reader.o link.o inventory.o collection.o
EXE = anthill
EXED = anthilldebug

all: (EXE)

(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) libscreen.a

game_loop.o: game_loop.c debug_printing.h command.h types.h game.h link.h \
 entity.h inventory.h object.h collection.h space.h player.h \
 game_reader.h game_actions.h graphic_engine.h
	$(CC) $(CFLAGS) -c game_loop.c

game.o: game.c debug_printing.h game.h link.h types.h entity.h \
 inventory.h object.h collection.h space.h command.h player.h
	$(CC) $(CFLAGS) -c game.c

command.o: command.c command.h types.h debug_printing.h
	$(CC) $(CFLAGS) -c command.c

space.o: space.c space.h types.h object.h link.h entity.h inventory.h \
 collection.h
	$(CC) $(CFLAGS) -c space.c

game_actions.o: game_actions.c game_actions.h command.h types.h game.h \
 link.h entity.h inventory.h object.h collection.h space.h player.h
	$(CC) $(CFLAGS) -c game_actions.c

graphic_engine.o: graphic_engine.c graphic_engine.h game.h link.h types.h \
 entity.h inventory.h object.h collection.h space.h command.h player.h \
 debug_printing.h libscreen.h
	$(CC) $(CFLAGS) -c graphic_engine.c

object.o: object.c object.h types.h
	$(CC) $(CFLAGS) -c object.c

entity.o: entity.c entity.h inventory.h types.h object.h collection.h \
 debug_printing.h
	$(CC) $(CFLAGS) -c entity.c
	
player.o: player.c player.h entity.h inventory.h types.h object.h \
 collection.h
	$(CC) $(CFLAGS) -c player.c

collector.o:	collector.c collector.h
	$(CC) $(CFLAGS) -c collector.c

debug_printing.o:	debug_printing.c debug_printing.h
	$(CC) $(CFLAGS) -c debug_printing.c

game_reader.o: game_reader.c game_reader.h types.h game.h link.h entity.h \
 inventory.h object.h collection.h space.h command.h player.h \
 debug_printing.h
	$(CC) $(CFLAGS) -c game_reader.c

link.o: link.c link.h types.h entity.h inventory.h object.h collection.h \
debug_printing.h
	$(CC) $(CFLAGS) -c link.c

inventory.o: inventory.c inventory.h types.h object.h collection.h \
debug_printing.h
	$(CC) $(CFLAGS) -c inventory.c

collection.o: collection.c collection.h types.h debug_printing.h
	$(CC) $(CFLAGS) -c collection.c


.PHONY:	clean compile run
clean:
	rm -f *.o $(EXE) debug.log $(EXED)

compile:
	$(CC) $(CFLAGS) -c $(SRC)

link:
	$(CC) $(CFLAGS) -o $(OBJ)

run:
	./anthill anthill.dat

debug:
	$(CC) $(CFLAGS) -g $(SRC) -o $(EXED) libscreen.a

runv:
	make debug
	valgrind --leak-check=full ./anthilldebug anthill.dat
	
# $@ devuelve lo que hay a la izquierda de los :, $^ devuelve todas las dependencias

# A la hora de añadir dependencias de librerias de otra carpeta se puede usar:
# por ejemplo si estoy compilando en src y hay una carpeta contigua llamada include
# gcc -I ../include (PARA BUSCAR EN LA CARPETA ../include)q