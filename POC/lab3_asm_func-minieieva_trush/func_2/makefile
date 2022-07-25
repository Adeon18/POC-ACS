# use makefile for automation of the compilation process. I just reused Dasha's make.
C = gcc
# Path to assembler
FS = ../fasmg/fasmg.x64
OBJ_D = obj
BIN_D = bin
SRC_D = src
FASM_INCLUDE = ../fasmg/include

all: create_dirs compile_asm

create_dirs:
	mkdir -p $(BIN_D)
	mkdir -p $(OBJ_D)

compile_asm:
	INCLUDE=$(FASM_INCLUDE) $(FS) $(SRC_D)/func.asm $(OBJ_D)/func.o
	INCLUDE=$(FASM_INCLUDE) $(FS) $(SRC_D)/main_a.asm $(OBJ_D)/main_a.o

	gcc $(SRC_D)/main_c.c $(OBJ_D)/func.o -o $(BIN_D)/call_from_c -no-pie -g
	gcc $(OBJ_D)/main_a.o $(OBJ_D)/func.o -o $(BIN_D)/call_from_s -no-pie -g

.PHONY : clean
clean:
	rm -rf bin/ obj/