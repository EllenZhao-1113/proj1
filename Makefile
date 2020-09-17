# Ellen Zhao
# Makefile
# COMP15
# Spring 2019
#
# Makefile for Tries
#

CC = clang++
CXXFLAGS = -std=c++11 -g3 -Wall -Wextra
DEPS = -g3

SeqMatch: main.o Tries.o
        ${CXX} ${LDFLAGS} -o SeqMatch main.o Tries.o

UnitTest: UnitTesting.o Tries.o
        ${CXX} ${LDFLAGS} -o UnitTest UnitTesting.o Tries.o

main.o: main.cpp Tries.h
Tries.o: Tries.cpp Tries.h

clean:
        rm -rf proj1 *.o *.dSYM

