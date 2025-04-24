#!/bin/bash

#for this script to function properly, the program structure must contain the following folders/structure
#./program/src/test/
#the script will be stored in ./program and the executable test files must be stored in ./program/src/test

test_dir="./src/test/"
valgrind="valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all "

if [ $# -ne 1 -a $# -ne 2 ] ; then
    echo "sintax is: $0 [0/1] [name_test]"
    echo "if name_test argument is left empty, all test's will be executed"
    exit 1
fi

if [ $1 -ne 0 -a $1 -ne 1 ] ; then
    echo "arg 1 is [0/1] to run all test without/with valgrind"
    exit 2
fi

if [ $# -eq 1 ] ; then
    if [ $1 -eq 0 ] ; then
        echo "Runinng test"
        make test_all
        for file in $(ls $test_dir)
        do
            if [ -d ${test_dir}$file ] ; then
                continue
            fi
            ${test_dir}$file
        done
        make clean_test
    elif [ $1 -eq 1 ] ; then
        echo "Running test with valgrind"
        make test_all
        for file in $(ls $test_dir)
        do
            if [ -d ${test_dir}$file ] ; then
                    continue
            fi
            ${valgrind}${test_dir}$file
        done
        make clean_test
    fi

elif [ $# -eq 2 ] ; then
    if [ $1 -eq 0 ] ; then
        echo "Running test $1"
        make ${test_dir}$2
        ${test_dir}$2
        make clean_test
    elif [ $1 -eq 1 ] ; then
        echo "Running test $1"
        make ${test_dir}$2
        ${valgrind}${test_dir}$2
        make clean_test
    fi
fi

exit 0