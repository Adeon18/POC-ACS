# Use makefile for automation of the compilation process.
# How to use it to call make in both subdirectories a.e.: https://stackoverflow.com/questions/2206128/how-to-call-makefile-from-another-makefile
C = gcc
FS = ../fasmg/fasmg.x64
OBJ_D = obj
BIN_D = bin
SRC_D = src
FASM_INCLUDE = ../fasmg/include

all:
	make -C func_1/ FASM_INCLUDE=$(FASM_INCLUDE) FS=$(FS)
	make -C func_2/ FASM_INCLUDE=$(FASM_INCLUDE) FS=$(FS)


.PHONY : clean
clean:
	make -C func_1/ clean
	make -C func_2/ clean
