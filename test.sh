#!/bin/bash

if [ $1 -eq 0 ]
    then
        echo "Runinng test"
        make test_all
        ./src/test/entity_test
        ./src/test/collection_test
        ./src/test/space_test
        ./src/test/link_test
        ./src/test/object_test
        ./src/test/inventory_test
        ./src/test/player_test
        make clean_test
else
    echo "Running test with valgrind"
    make test_all
    valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./src/test/entity_test
    valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./src/test/collection_test
    valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./src/test/space_test
    valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./src/test/link_test
    valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./src/test/object_test
    valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./src/test/inventory_test
    valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./src/test/player_test
    make clean_test
fi