# ___MAKEFILE___

# You can run make help to get more information about targets

# Compiler, linker and flags

CC = gcc
CFLAGS = -g -Wall -Wextra -O3
LDFLAGS = -Wall -I$(DINC)

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

.PHONY: clean cleanSoft

clean:
	@rm -rf $(DOBJ) $(DBIN)
	@echo "Clean up project directories"

cleanSoft:
	@rm -f $(OBJ) $(EXE)
	@echo "Objects and executable removed"

help:
	@echo -e "\n\t\t\x1b[1mHUFFMAN CODING\x1b[0m\n\n"
	@echo -e "\tFew explanations of target rules with make command.\n\tBy default, without rule, make will build the project.\n"
	@echo -e "\t\x1b[1mUSAGE\x1b[0m"
	@echo -e "\t\tmake [RULE]\n"
	@echo -e "\t\x1b[1mLIST OF RULES\n" 
	@echo -e "\t\t\x1b[1mclean\x1b[0m\t\tremove object and binary directories with their content." 
	@echo -e "\t\t\x1b[1mcleanSoft\x1b[0m\tonly remove content of objects and binary directories.\n"