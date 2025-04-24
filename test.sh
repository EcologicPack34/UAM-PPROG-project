#!/bin/bash

test_dir="./src/test/"
valgrind="valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all "


echo "$test_dir"

if [ $# -eq 0 ]
    then
        echo "sintax is: $0 [0/1]"
        echo "0 to run all test without valgrind"
        echo "1 to run all test with valgrind"
        exit 1
fi

if [ $1 -eq 0 ]
    then
        echo "Runinng test"
        make test_all
        for file in $(ls $test_dir)
        do
            if [ -d ${test_dir}$file ]
                then
                    continue
            fi
            ${test_dir}$file
        done
        make clean_test
else
    if [ $1 -eq 1 ]
    then
        echo "Running test with valgrind"
        make test_all
        for file in $(ls $test_dir)
        do
            if [ -d ${test_dir}$file ]
                then
                    continue
            fi
            ${valgrind}${test_dir}$file
        done
        make clean_test
    else
        echo "Invalid argument. Use 0 for normal execution, 1 for valgrind"
    fi
fi