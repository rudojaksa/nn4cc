# env needed to export the path to the $(shell something) inside recipes
export  PATH := .:$(PATH):../UTIL
export SHELL := env PATH=$(PATH) /bin/bash

T    := nntrain

SRC  := $(T:%=%.c)
README := $(T:%=%.md)
LIB  := $(shell find . -type f -name '*.c' | grep -v './OLD/' | grep -v './test/' | sed 's:^\./::' | sort)
LIB  := $(filter-out $(SRC),$(LIB))
OBJ  := $(LIB:%.c=%.o)
OBJ  := $(filter-out help.o,$(OBJ)) help.o
DEP  := $(OBJ:%.o=%.d)

ifneq ($(shell gcc --version 2>&1 | grep clang),)
  ISCLANG := 1
endif

# CC WARNINGS FLAGS
ifdef ISCLANG
# W += -Wno-unused-variable
  W += -Wno-empty-body
  W += -Wno-varargs
else
  W += -Wno-misleading-indentation
endif

# OPTIMIZATION FLAGS
ifdef ISCLANG
  O := -O3 -ffast-math
else
# O := -O3 -ffast-math
# O := -Ofast -march=native -mavx2 -mfma
  O := -Ofast -march=native
# O += -fopenmp -pthread
# O += -ftree-parallelize-loops=4
endif

# VERBOSE
ifndef ISCLANG
  O += -fopt-info # -fopt-info-all # -fopt-info-all-internals
# O += -fopt-info-inline-all-internals # verbose inline info
# O += -fopt-info-vec -fopt-info-vec-all # -fopt-info-vec-all-internals
# O += -fopt-info-vec-all-internals # -fopt-info-omp-all-internals
endif

# PROFILE BASED OPTIMIZATION
# O += -fprofile-generate=profile
# O += -fprofile-use=profile

# DEBUG
ifdef DEBUG
  O := $(XFLAGS) -O1 -g -fsanitize=address
# O := $(XFLAGS) -O1 -g
endif

# CFLAGS
CFLAGS := -std=gnu99 -Wall $W $O

all: $T

$T: %: %.c $(OBJ) Makefile | VERSION.h
	@gccdoctor $(CFLAGS) -o $@ $< $(OBJ) -lm -lpthread

$(OBJ): %.o: %.c Makefile | VERSION.h
	@gccdoctor -MMD -MP $(CFLAGS) -c -o $@ $<

VERSION.h: $(SRC) VERSION Makefile ../UTIL/mkversionh
	mkversionh > $@

help.c: help.txt CONFIG.h VERSION.h sugar.c sched.c
	help2c $< > $@

# TODO: nntrain should be installed as a symbolic link to nntrain in this directory
#       to stay allways freshly recompiled to the latest topology

install: all
	makeinstall -f $T

clean:
	rm -fv aux/*.d
	rm -fv aux/*.o
	rm -fv *.d
	rm -fv *.o
	rm -fv $T
	rm -fv VERSION.h

-include $(DEP)

