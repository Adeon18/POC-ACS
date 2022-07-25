# C Compiler`
CC ?= gcc
# Some flags
CCFLAGS ?= -O3
LDFLAGS ?= -O3
# Lib name
PRG_NAME = libzlib
# Lib extentions
DLIBEXT = .so
SLIBEXT = .a
# Output Directory Names
BIN_DIR = bin
OBJ_DIR = obj
FPIC_OBJ_DIR = obj_fpic
DEF_OBJ_DIR = obj_no_fpic
# Source files and Header files
SOURCES = adler32.c crc32.c gzclose.c gzread.c infback.c inflate.c trees.c zutil.c compress.c deflate.c gzlib.c gzwrite.c inffast.c inftrees.c uncompr.c
HEADERS = crc32.h gzguts.h inffixed.h inftrees.h zconf.h zutil.h deflate.h inffast.h inflate.h trees.h zlib.h
INCLUDE = -I ../library
# The same list but with Object files instead of sources
FPIC_OBJS = $(patsubst %.c,$(OBJ_DIR)/$(FPIC_OBJ_DIR)/%.o,$(SOURCES))
DEF_OBJS = $(patsubst %.c,$(OBJ_DIR)/$(DEF_OBJ_DIR)/%.o,$(SOURCES))

all: create_dirs $(BIN_DIR)/$(PRG_NAME)$(DLIBEXT) $(BIN_DIR)/$(PRG_NAME)$(SLIBEXT)
# Create Directories
create_dirs:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)/$(FPIC_OBJ_DIR)
	mkdir -p $(OBJ_DIR)/$(DEF_OBJ_DIR)

# Compile the main dynamic library
$(BIN_DIR)/$(PRG_NAME)$(DLIBEXT): $(FPIC_OBJS)
	$(CC) -shared -fPIC $^ -o $@ $(LDFLAGS)

# Compile the main static library
$(BIN_DIR)/$(PRG_NAME)$(SLIBEXT): $(DEF_OBJS)
	ar rcs $@ $^

# Compile each .o file into OBJ_DIR/FPIC_OBJ_DIR. Compile with -fPIC!
$(OBJ_DIR)/$(FPIC_OBJ_DIR)/%.o: %.c 
	$(CC) -fPIC -c $< $(INCLUDE) -o $(OBJ_DIR)/$(FPIC_OBJ_DIR)/$*.o $(CCFLAGS)

# Compile each .o file into OBJ_DIR/DEF_OBJ_DIR.
$(OBJ_DIR)/$(DEF_OBJ_DIR)/%.o: %.c
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)/$(DEF_OBJ_DIR)/$*.o $(CCFLAGS)


.PHONY : clean create_dirs
clean:
	rm -rf $(OBJ_DIR)/$(FPIC_OBJ_DIR)/*.o $(OBJ_DIR)/$(DEF_OBJ_DIR)/*.o $(BIN_DIR)/$(PRG_NAME)$(DLIBEXT) $(BIN_DIR)/$(PRG_NAME)$(SLIBEXT)

