# C Compiler
CC ?= gcc
# Some flags
CCFLAGS ?= -O3
LDFLAGS ?= -O3
# Lib name
PRG_NAME1 = example
PRG_NAME2 = minigzip
# Output Directory Names
BIN_DIR = bin
OBJ_DIR = obj
# VPATH for the headers
vpath %.h ./../library
# Source files and Header files	
SOURCES = example.c minigzip.c
HEADERS = zlib.h
INCLUDE = -I ../library
# The same list but with Object files instead of sources
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SOURCES))

all: create_dirs $(BIN_DIR)/$(PRG_NAME1) $(BIN_DIR)/$(PRG_NAME2)
# Create Directories
create_dirs:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

# Link the example file to the dynamic lib TODO: Make it work not only from the example dir
$(BIN_DIR)/$(PRG_NAME1): $(OBJ_DIR)/example.o
	$(CC) $^ $(LDFLAGS) -o $(BIN_DIR)/$(PRG_NAME1) -L../library/bin -lzlib
# The same but for the other example. They are separated because I felt it would be better for debug :D
$(BIN_DIR)/$(PRG_NAME2): $(OBJ_DIR)/minigzip.o
	$(CC) $^ $(LDFLAGS) -o $(BIN_DIR)/$(PRG_NAME2) -L../library/bin -lzlib

# Compile each .o file into OBJ_DIR. Compile with -fPIC!
$(OBJ_DIR)/%.o: %.c
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)/$*.o $(CCFLAGS)

.PHONY : clean create_dirs
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/$(PRG_NAME)

