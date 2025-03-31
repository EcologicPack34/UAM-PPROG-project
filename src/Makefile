CC = gcc

CFLAGS = -Wall -pedantic 
SRC = collector.c command.c debug_printing.c entity.c game_actions.c game_loop.c game.c graphic_engine.c space.c object.c player.c game_reader.c  \
 link.c inventory.c collection.c event_manager.c event_actions.c npc.c vector2.c queue.c message.c combat.c

OBJ = collector.o command.o debug_printing.o entity.o game_actions.o game_loop.o game.o graphic_engine.o space.o object.o player.o libscreen.a \
 game_reader.o link.o inventory.o collection.o event_manager.o event_actions.o npc.o vector2.o queue.o message.o combat.o

EXE = anthill
EXED = anthilldebug

TEST_OBJ = entity_test.o collection_test.o
TEST = entity_test collection_test

all: (EXE)

(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) libscreen.a

game_loop.o: game_loop.c debug_printing.h command.h types.h game.h link.h \
 entity.h inventory.h object.h collection.h space.h npc.h vector2.h \
 player.h event_manager.h message.h combat.h queue.h game_reader.h \
 game_actions.h graphic_engine.h event_actions.h
	$(CC) $(CFLAGS) -c game_loop.c

game.o: game.c game.h link.h types.h entity.h inventory.h object.h \
 collection.h space.h npc.h debug_printing.h vector2.h command.h player.h \
 event_manager.h message.h combat.h queue.h
	$(CC) $(CFLAGS) -c game.c

command.o: command.c command.h types.h debug_printing.h
	$(CC) $(CFLAGS) -c command.c

space.o: space.c space.h types.h object.h link.h entity.h inventory.h \
 collection.h npc.h debug_printing.h vector2.h
	$(CC) $(CFLAGS) -c space.c

game_actions.o: game_actions.c game_actions.h game.h link.h types.h \
 entity.h inventory.h object.h collection.h space.h npc.h \
 debug_printing.h vector2.h command.h player.h event_manager.h message.h \
 combat.h queue.h
	$(CC) $(CFLAGS) -c game_actions.c

graphic_engine.o: graphic_engine.c graphic_engine.h game.h link.h types.h \
 entity.h inventory.h object.h collection.h space.h npc.h \
 debug_printing.h vector2.h command.h player.h event_manager.h message.h \
 combat.h queue.h libscreen.h
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
 inventory.h object.h collection.h space.h npc.h debug_printing.h \
 vector2.h command.h player.h event_manager.h message.h combat.h queue.h
	$(CC) $(CFLAGS) -c game_reader.c

link.o: link.c link.h types.h entity.h inventory.h object.h collection.h \
 debug_printing.h
	$(CC) $(CFLAGS) -c link.c

inventory.o: inventory.c inventory.h types.h object.h collection.h \
 debug_printing.h
	$(CC) $(CFLAGS) -c inventory.c

collection.o: collection.c collection.h types.h debug_printing.h
	$(CC) $(CFLAGS) -c collection.c

event_manager.o: event_manager.c event_manager.h command.h types.h \
 collection.h debug_printing.h
	$(CC) $(CFLAGS) -c event_manager.c

event_actions.o: event_actions.c event_actions.h event_manager.h \
 command.h types.h game.h link.h entity.h inventory.h object.h \
 collection.h space.h npc.h debug_printing.h vector2.h player.h message.h \
 combat.h queue.h
	$(CC) $(CFLAGS) -c event_actions.c

npc.o: npc.c npc.h entity.h inventory.h types.h object.h collection.h \
 debug_printing.h
	$(CC) $(CFLAGS) -c npc.c

queue.o: queue.c queue.h types.h
	$(CC) $(CFLAGS) -c queue.c

vector2.o: vector2.c vector2.h
	$(CC) $(CFLAGS) -c vector2.c

message.o: message.c message.h
	$(CC) $(CFLAGS) -c message.c

combat.o: combat.c combat.h collection.h types.h debug_printing.h queue.h \
 entity.h inventory.h object.h npc.h space.h link.h vector2.h player.h \
 command.h
	$(CC) $(CFLAGS) -c combat.c

.PHONY:	clean compile link run debug runv
clean:
	@rm -f *.o $(EXE) debug.log $(EXED) $(TEST) $(TEST_OBJ)
	@cd ./test
	@rm -f debug.log $(TEST)
	@cd ..

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
	valgrind --leak-check=full --track-origins=yes ./anthilldebug anthill.dat

run_entity_test:
	make
	$(CC) $(CFLAGS) -c ./test/entity_test.c
	$(CC) $(CFLAGS) -o ./test/entity_test entity_test.o entity.o inventory.o object.o collection.o debug_printing.o
	./test/entity_test

run_collection_test:
	make
	$(CC) $(CFLAGS) -c ./test/collection_test.c
	$(CC) $(CFLAGS) -o ./test/collection_test collection_test.o collection.o debug_printing.o
	./test/collection_test

run_space_test:
	make
	$(CC) $(CFLAGS) -c ./test/space_test.c
	$(CC) $(CFLAGS) -o ./test/space_test space_test.o space.o inventory.o collection.o debug_printing.o object.o link.o npc.o entity.o vector2.o
	./test/space_test
	
# $@ devuelve lo que hay a la izquierda de los :, $^ devuelve todas las dependencias

# A la hora de añadir dependencias de librerias de otra carpeta se puede usar:
# por ejemplo si estoy compilando en src y hay una carpeta contigua llamada include
# gcc -I ../include (PARA BUSCAR EN LA CARPETA ../include)q