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

EXE = anthill
EXED = anthilldebug

TEST_OBJ = entity_test.o collection_test.o space_test.o link_test.o object_test.o inventory_test.o
TEST = entity_test collection_test space_test link_test object_test inventory_test


all: $(EXE)

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

#Rule to link all the objects with libraries
$(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -L$(LIBRARIES) -lscreen

#Rule to compile each .c file into its .o file
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	$(CC) -c -o $@ $< $(CFLAGS)

-include $(DEPENDENCIES)

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
	gdb --tui --silent --args ./anthilldebug anthill.dat

#To install ddd run $sudo apt install ddd
ddd:
	make debug
	ddd --gdb --args ./anthilldebug anthill.dat

#in order for doxygen to work run $sudo apt install doxygen
#for graphs to work it needs graphviz, to install run $sudo apt install graphviz
doxy:
	@doxygen Doxyfile

runv:
	make debug
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./anthilldebug anthill.dat

#tests rules

#in order to make the objects less hard to read, i'll use the following variable
OP = $(OBJ_PATH)

LINK_TEST_OBJ = ./$(OP)/link_test.o ./$(OP)/link.o ./$(OP)/ability_manager.o ./$(OP)/queue.o ./$(OP)/entity.o ./$(OP)/object.o ./$(OP)/inventory.o ./$(OP)/collection.o ./$(OP)/debug_printing.o
SPACE_TEST_OBJ  = ./$(OP)/space_test.o ./$(OP)/space.o ./$(OP)/ability_manager.o ./$(OP)/queue.o ./$(OP)/inventory.o ./$(OP)/collection.o ./$(OP)/debug_printing.o ./$(OP)/object.o ./$(OP)/link.o ./$(OP)/npc.o ./$(OP)/entity.o ./$(OP)/vector2.o
OBJECT_TEST_OBJ = ./$(OP)/object_test.o ./$(OP)/object.o ./$(OP)/ability_manager.o ./$(OP)/queue.o ./$(OP)/debug_printing.o ./$(OP)/collection.o
INVENTORY_TEST_OBJ = ./$(OP)/inventory_test.o ./$(OP)/inventory.o ./$(OP)/collection.o ./$(OP)/object.o ./$(OP)/debug_printing.o
ENTITY_TEST_OBJ = ./$(OP)/entity.o ./$(OP)/inventory.o ./$(OP)/object.o ./$(OP)/collection.o ./$(OP)/debug_printing.o ./$(OP)/ability_manager.o ./$(OP)/queue.o
COLLECTION_TEST_OBJ = ./$(OP)/collection_test.o ./$(OP)/collection.o ./$(OP)/debug_printing.o
PLAYER_TEST_OBJ = ./$(OP)/player_test.o ./$(OP)/player.o ./$(OP)/command.o ./$(OP)/equipment.o ./$(OP)/npc.o ./$(OP)/entity.o ./$(OP)/object.o ./$(OP)/debug_printing.o ./$(OP)/inventory.o ./$(OP)/collection.o ./$(OP)/ability_manager.o ./$(OP)/queue.o

run_test_all: run_collection_test run_space_test run_link_test run_object_test run_inventory_test run_player_test clean

run_entity_test:
	make
	$(CC) -Wall -pedantic -I$(INCLUDE) -c ./src/test/src/entity_test.c -o ./$(OBJ_PATH)/entity_test.o 
	$(CC) $(CFLAGS) -o ./src/test/link_test $(ENTITY_TEST_OBJ)
	./src/test/entity_test

run_collection_test:
	make
	$(CC) -Wall -pedantic -I$(INCLUDE) -c ./src/test/src/collection_test.c -o ./$(OBJ_PATH)/collection_test.o 
	$(CC) $(CFLAGS) -o ./src/test/collection_test $(COLLECTION_TEST_OBJ)
	./src/test/collection_test

run_space_test:
	make
	$(CC) -Wall -pedantic -c ./src/test/src/space_test.c -o ./$(OBJ_PATH)/space_test.o 
	$(CC) $(CFLAGS) -o ./src/test/space_test $(SPACE_TEST_OBJ)
	./src/test/space_test

run_link_test:
	make
	$(CC) -Wall -pedantic -I$(INCLUDE) -c ./src/test/src/link_test.c -o ./$(OBJ_PATH)/link_test.o 
	$(CC) $(CFLAGS) -o ./src/test/link_test $(LINK_TEST_OBJ)
	./src/test/link_test

run_object_test:
	make
	$(CC) -Wall -pedantic -I$(INCLUDE) -c ./src/test/src/object_test.c -o ./$(OBJ_PATH)/object_test.o 
	$(CC) $(CFLAGS) -o ./src/test/object_test $(OBJECT_TEST_OBJ)
	./src/test/object_test

run_inventory_test:
	make
	$(CC) -Wall -pedantic -I$(INCLUDE) -c ./src/test/src/inventory_test.c -o ./$(OBJ_PATH)/inventory_test.o 
	$(CC) $(CFLAGS) -o ./src/test/inventory_test $(INVENTORY_TEST_OBJ)
	./src/test/inventory_test

run_player_test:
	make
	$(CC) -g -Wall -pedantic -I$(INCLUDE) -c ./src/test/src/player_test.c -o ./$(OBJ_PATH)/player_test.o 
	$(CC) $(CFLAGS) -o ./src/test/player_test $(PLAYER_TEST_OBJ)
	./src/test/player_test
	


# $@ devuelve lo que hay a la izquierda de los :, $^ devuelve todas las dependencias

# A la hora de añadir dependencias de librerias de otra carpeta se puede usar:
# por ejemplo si estoy compilando en src y hay una carpeta contigua llamada include
# gcc -I ../include (PARA BUSCAR EN LA CARPETA ../include)q
