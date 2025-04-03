CC = gcc

INCLUDE = include
LIBRARIES = libraries
OBJ_PATH = object
SRC_PATH = src
DEP_PATH = dependency


#-MMP generates dependencies files for each .c, i.e. it generates a file that makefile checks to see its dependencies when compiling
#-MP Is used to detect errors if any .h is deleted, but mentioned on a dependency file (.d)
CFLAGS = -Wall -pedantic -I$(INCLUDE) -MMD -MP

D_FLAGS = -Wall -pedantic -g -I$(INCLUDE)

#Names of the .c files
_SRC = command.c debug_printing.c game_actions.c game_loop.c game.c graphic_engine.c space.c object.c player.c game_reader.c entity.c \
 link.c inventory.c collection.c event_manager.c event_actions.c npc.c vector2.c queue.c message.c combat.c ability_actions.c ability_manager.c \
 equipment.c

#Adds to the names of .c files the path of the file before
SRC = $(patsubst %,$(SRC_PATH)/%,$(_SRC))

#changes the .c extension from SRC to .o extension
_OBJ = $(_SRC:.c=.o)

#Like in SRC, we copy the OBJ_PATH before each object
OBJ = $(patsubst %,$(OBJ_PATH)/%,$(_OBJ))

#changes the .o extension from OBJ to .d extension to include the dependencies
DEPENDENCIES = $(OBJ:.o=.d)
#includes the dependencies so that they are considered when compiling
-include $(DEPENDENCIES)

EXE = anthill
EXED = anthilldebug

TEST_OBJ = entity_test.o collection_test.o
TEST = entity_test collection_test


all: $(EXE)

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

#Rule to link all the objects with libraries
$(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -L$(LIBRARIES) -lscreen

#Rule to compile each .c file into its .o file
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY:	clean compile link run runl debug runv gdb
clean:
	@cd ./$(OBJ_PATH)
	@rm -f $(OBJ) $(DEPENDENCIES) $(EXE) debug.log $(EXED) $(TEST) $(TEST_OBJ)
	@cd ..
	@echo "Removed all *.o, executables and logs"
#	@cd ./test
#	@rm -f debug.log $(TEST)
#	@cd ..

compile:
	$(CC) $(CFLAGS) -c $(SRC)

link:
	$(CC) $(CFLAGS) -o $(OBJ)

run:
	./anthill anthill.dat

runl:
	./anthill anthill.dat -l ./debug.log

debug:
	$(CC) -o $(EXED) $(SRC) $(D_FLAGS) -g -L$(LIBRARIES) -lscreen

gdb:
	make debug
	gdb --tui --silent ./anthilldebug

runv:
	make debug
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./anthilldebug anthill.dat

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
