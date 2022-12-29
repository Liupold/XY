MAKEFLAGS := --jobs=$(shell nproc)

CC = gcc
CFLAGS = -march=native -O3 -Wall -Wextra -Wpedantic -Ofast -fopenmp -Wconversion \
		 -std=c99 -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual \
		 -Wstrict-prototypes -Wdeclaration-after-statement -lm -mavx# -Werror

LIB_NAME = nd-xy
DIR_EXPT = expt
DIR_LIB = lib
DIR_BINs = bins
DIR_INCLUDE = include
src_progs := $(shell find $(DIR_EXPT) -name '*.c' | sed "s/.*\//$(DIR_BINs)\//; s/\.c//")

xy-experiments: $(DIR_BINs) $(src_progs)

$(DIR_LIB)/lib$(LIB_NAME).so: $(DIR_LIB)/nd-xy.c
	$(CC) $(CFLAGS) -I$(DIR_INCLUDE) -shared $(DIR_LIB)/*.c -o $(DIR_LIB)/lib$(LIB_NAME).so

$(DIR_BINs)/%: visual/%.c $(DIR_LIB)/lib$(LIB_NAME).so
	$(CC) $(CFLAGS) -I$(DIR_INCLUDE) -L$(DIR_LIB) $< -l:lib$(LIB_NAME).so -lm -Wl,-rpath=$(realpath $(DIR_LIB)) -o $@ -lGL -lglut -lGLU

$(DIR_BINs)/%: $(DIR_EXPT)/%.c $(DIR_LIB)/lib$(LIB_NAME).so
	$(CC) $(CFLAGS) -I$(DIR_INCLUDE) -L$(DIR_LIB) $< -l:lib$(LIB_NAME).so -lm -Wl,-rpath=$(realpath $(DIR_LIB)) -o $@

$(DIR_BINs):
	mkdir -p $(DIR_BINs)

clean:
	rm -rf $(DIR_BUILD) $(DIR_BINs)
	rm $(DIR_LIB)/lib$(LIB_NAME).so
