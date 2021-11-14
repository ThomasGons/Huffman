# Compiler, linker and flags

CC = gcc
CFLAGS = -Wall -Wextra -O3
LDFLAGS = -Wall -lm -I$(DINC)

# Directories

DSRC = src/
DINC = inc/
DOBJ = obj/
DBIN = bin/

# Files

SRC = $(wildcard $(DSRC)*.c)
INC = $(wildcard $(DINC)*.h)
OBJ = $(SRC:$(DSRC)%.c=$(DOBJ)%.o)

TARGET = $(DBIN)exe

all: $(TARGET)
	
$(TARGET): $(OBJ)
	@mkdir -p $(DBIN)
	$(CC) $^ $(LFLAGS) -o $@
	@echo "Linking finished !"

$(DOBJ)%.o: $(DSRC)%.c
	@mkdir -p $(DOBJ)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Successful compilation !"

.PHONY: clean init

clean:
	@rm -rf $(DOBJ) $(DBIN)
	@echo "Objects and executable removed"