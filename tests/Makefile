EXE := mfxp_test

SRC_DIR := ../src
SRC_DIR_TEST := ./src
OBJ_DIR := obj

#SRC := $(wildcard $(SRC_DIR_TEST)/*.c) $(wildcard $(SRC_DIR)/*.c) $(wildcard ../libs/inih/*.c)
SRC := $(filter-out main.c, $(notdir $(shell find $(SRC_DIR) -name "*.c"))) $(notdir $(shell find $(SRC_DIR_TEST) -name "*.c"))
#$(warning $(SRC))
OBJ := $(SRC:%.c=$(OBJ_DIR)/%.o)
#$(warning $(OBJ))
CPPFLAGS := -I../include -I./include
CFLAGS   := -std=gnu99 -Wall -g
LDLIBS   := -lssl -lcrypto -lbsd -lreadline -lpthread

.PHONY: all clean tags

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR_TEST)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $@

clean:
	$(RM) $(OBJ) $(EXE)
