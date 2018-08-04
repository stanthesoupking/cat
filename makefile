CFLAG =
CC = gcc
INCLUDE = cmdlib.h
LIBS =
INPUT = cat.c cmdlib.c
OUTPUT = cat

run:
	${CC} ${INPUT} ${CFLAGS} ${INCLUDE} -o cat